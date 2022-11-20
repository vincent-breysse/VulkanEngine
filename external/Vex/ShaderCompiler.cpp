
#include <pch.hpp>
#include <cassert>
#include <map>
#include <algorithm>
#include <shaderc/shaderc.hpp>
#include <SPIRV-Reflect/spirv_reflect.h>
#include <Vex/ShaderCompiler.hpp>

namespace
{
    struct BindingSlotKey
    {
        uint32_t set;
        uint32_t binding;
    };

    struct BindingSlotData
    {
        const SpvReflectDescriptorBinding* spvBinding;
        VkShaderStageFlags accessMask;
    };
}

namespace std
{
    template<>
    struct less<BindingSlotKey>
    {
        bool operator()(const BindingSlotKey& a, const BindingSlotKey& b) const
        {
            if (a.set != b.set)
            {
                return a.set < b.set;
            }
            else
            {
                return a.binding < b.binding;
            }
        }
    };
}

namespace shc = shaderc;

static shaderc_optimization_level ToShadercOptimizationLevel(VexShaderOptimizationProfile profile)
{
    switch (profile)
    {
    case VexShaderOptimizationProfile::None: return shaderc_optimization_level_zero;
    case VexShaderOptimizationProfile::MinSize: return shaderc_optimization_level_size;
    case VexShaderOptimizationProfile::MaxSpeed: return shaderc_optimization_level_performance;
    }

    assert(false);
    return shaderc_optimization_level(0);
}

static VkShaderStageFlagBits ToShaderStageFlags(shaderc_shader_kind shaderKind)
{
    switch (shaderKind)
    {
    case shaderc_vertex_shader: return VK_SHADER_STAGE_VERTEX_BIT;
    case shaderc_fragment_shader: return VK_SHADER_STAGE_FRAGMENT_BIT;
    }

    assert(false);
    return VkShaderStageFlagBits(0);
}

static VexShaderStageCompilation CompileShaderStage(shc::Compiler& compiler, const char* source, size_t sourceSize, shaderc_shader_kind shaderKind, const char* sourceTag, const char* entryPointName, const shc::CompileOptions& options)
{
    if (source == nullptr)
        return {};

    shc::CompilationResult scResult = compiler.CompileGlslToSpv(source, sourceSize, shaderKind, sourceTag, entryPointName, options);
    std::vector<uint32_t> spirvCode(scResult.cbegin(), scResult.cend());
    uint32_t spirvCodeSize = uint32_t(spirvCode.size() * sizeof(uint32_t));

    VexShaderStageCompilation ret;
    ret.stage = ToShaderStageFlags(shaderKind);
    ret.spirvCode = std::move(spirvCode);
    ret.spirvCodeSize = spirvCodeSize;
    ret.errorCount = uint32_t(scResult.GetNumErrors());
    ret.warningCount = uint32_t(scResult.GetNumWarnings());
    ret.errorMessage = scResult.GetErrorMessage();

    return ret;
}

static SpvReflectShaderModule ExtractBindingSlotDataFromShaderStage(const void* spirvCode, uint32_t codeSize, VkShaderStageFlagBits shaderStageMask, std::map<BindingSlotKey, BindingSlotData>& slotMap)
{
    if (spirvCode == nullptr)
        return {};

    SpvReflectResult result = SpvReflectResult(0);

    SpvReflectShaderModule module;
    result = spvReflectCreateShaderModule(codeSize, spirvCode, &module);
    assert(result == SPV_REFLECT_RESULT_SUCCESS);

    uint32_t count;
    result = spvReflectEnumerateDescriptorBindings(&module, &count, nullptr);
    assert(result == SPV_REFLECT_RESULT_SUCCESS);

    std::vector<SpvReflectDescriptorBinding*> bindings(count);
    result = spvReflectEnumerateDescriptorBindings(&module, &count, bindings.data());
    assert(result == SPV_REFLECT_RESULT_SUCCESS);

    for (uint32_t i = 0; i < count; ++i)
    {
        SpvReflectDescriptorBinding& spvBinding = *bindings[i];

        BindingSlotKey key;
        key.set = spvBinding.set;
        key.binding = spvBinding.binding;

        BindingSlotData& data = slotMap[key];
        data.spvBinding = &spvBinding;
        if (spvBinding.accessed != 0)
        {
            data.accessMask |= shaderStageMask;
        }
    }

    return module;
}

static std::vector<VexReflectDescriptorSetBinding> BuildBindingsReflectionDataFromSlotMap(const std::map<BindingSlotKey, BindingSlotData>& slotMap)
{
    std::vector<VexReflectDescriptorSetBinding> bindings;
    bindings.reserve(slotMap.size());

    for (const auto& kv : slotMap)
    {
        const BindingSlotData& slot = kv.second;
        const SpvReflectDescriptorBinding& spvBinding = *slot.spvBinding;

        const char* typeName = spvBinding.type_description->type_name;

        VexReflectDescriptorSetBinding& binding = bindings.emplace_back();
        binding.typeName = typeName ? std::string(typeName) : "";
        binding.name = spvBinding.name ? std::string(spvBinding.name) : "";
        binding.binding = spvBinding.binding;
        binding.set = spvBinding.set;
        binding.count = spvBinding.count;
        binding.type = VkDescriptorType(spvBinding.descriptor_type);
        binding.accessMask = slot.accessMask;
    }

    return bindings;
}

static std::vector<VexReflectDescriptorSetBinding> BuildBindingsReflectionData(const VexShaderStageCompilation& vertexStage, const VexShaderStageCompilation& fragStage)
{
    std::map<BindingSlotKey, BindingSlotData> slotMap;

    SpvReflectShaderModule vertexModule = ExtractBindingSlotDataFromShaderStage(vertexStage.spirvCode.data(), vertexStage.spirvCodeSize, VK_SHADER_STAGE_VERTEX_BIT, slotMap);
    SpvReflectShaderModule fragModule = ExtractBindingSlotDataFromShaderStage(fragStage.spirvCode.data(), fragStage.spirvCodeSize, VK_SHADER_STAGE_FRAGMENT_BIT, slotMap);

    std::vector<VexReflectDescriptorSetBinding> bindings = BuildBindingsReflectionDataFromSlotMap(slotMap);

    spvReflectDestroyShaderModule(&vertexModule);
    spvReflectDestroyShaderModule(&fragModule);

    return bindings;
}

static std::vector<VexReflectDescriptorSet> BuildDescriptorSetReflectionDataFromBindings(const std::vector<VexReflectDescriptorSetBinding>& bindings)
{
    std::vector<uint32_t> bindingCountPerSet;
    bindingCountPerSet.reserve(bindings.size());

    for (const VexReflectDescriptorSetBinding& binding : bindings)
    {
        if (binding.set >= bindingCountPerSet.size())
        {
            bindingCountPerSet.resize(binding.set + 1);
        }

        ++bindingCountPerSet[binding.set];
    }

    std::vector<VexReflectDescriptorSet> dscSets;
    dscSets.reserve(bindings.size());

    uint32_t rangeBegin = 0;
    for (size_t i = 0; i < bindingCountPerSet.size(); ++i)
    {
        uint32_t bindingCount = bindingCountPerSet[i];

        if (bindingCount == 0)
            continue;

        uint32_t rangeEnd = rangeBegin + bindingCount;

        VexReflectDescriptorSet& dscSet = dscSets.emplace_back();
        dscSet.set = uint32_t(i);
        dscSet.rangeBegin = rangeBegin;
        dscSet.rangeEnd = rangeEnd;

        rangeBegin = rangeEnd;
    }

    dscSets.shrink_to_fit();
    return dscSets;
}

static VexShaderReflection ExtractShaderReflection(const VexShaderStageCompilation& vertexStage, const VexShaderStageCompilation& fragStage)
{
    std::vector<VexReflectDescriptorSetBinding> bindings = BuildBindingsReflectionData(vertexStage, fragStage);
    std::vector<VexReflectDescriptorSet> dscSets = BuildDescriptorSetReflectionDataFromBindings(bindings);

    VexShaderReflection reflection;
    reflection.bindings = std::move(bindings);
    reflection.descriptorSets = std::move(dscSets);

    return reflection;
}

static void SetKeywordsToCompileOptions(shc::CompileOptions& options, const VexShaderKeyword* keywords, uint32_t keywordCount)
{
    for (uint32_t i = 0; i < keywordCount; ++i)
    {
        const char* name = keywords[i].name;
        const char* value = keywords[i].value;

        if (value != nullptr)
        {
            options.AddMacroDefinition(name, value);
        }
        else
        {
            options.AddMacroDefinition(name);
        }
    }
}

VexShaderCompilation VexCompileShader(const VexShaderCompilationInput* info)
{
    shaderc_optimization_level shcOptLevel = ToShadercOptimizationLevel(info->optimizationProfile);

    shc::Compiler compiler;

    shc::CompileOptions baseOptions;
    baseOptions.SetOptimizationLevel(shaderc_optimization_level_zero);

    shc::CompileOptions vertexOptions = baseOptions;
    SetKeywordsToCompileOptions(vertexOptions, info->vertexKeywords, info->vertexKeywordCount);
    VexShaderStageCompilation vertexStage = CompileShaderStage(compiler, info->vertexSource, info->vertexSourceSize, shaderc_vertex_shader, info->sourceTag, info->entryPointName, vertexOptions);

    shc::CompileOptions fragOptions = baseOptions;
    SetKeywordsToCompileOptions(fragOptions, info->fragKeywords, info->fragKeywordCount);
    VexShaderStageCompilation fragStage = CompileShaderStage(compiler, info->fragSource, info->fragSourceSize, shaderc_fragment_shader, info->sourceTag, info->entryPointName, fragOptions);

    VexShaderReflection reflection = ExtractShaderReflection(vertexStage, fragStage);

    vertexOptions.SetOptimizationLevel(shcOptLevel);
    vertexStage = CompileShaderStage(compiler, info->vertexSource, info->vertexSourceSize, shaderc_vertex_shader, info->sourceTag, info->entryPointName, vertexOptions);

    fragOptions.SetOptimizationLevel(shcOptLevel);
    fragStage = CompileShaderStage(compiler, info->fragSource, info->fragSourceSize, shaderc_fragment_shader, info->sourceTag, info->entryPointName, fragOptions);

    VexShaderCompilation ret;
    ret.vertex = std::move(vertexStage);
    ret.frag = std::move(fragStage);
    ret.reflection = std::move(reflection);

    return ret;
}

VexShaderCompiler::VexShaderCompiler()
{
    memset(this, 0, sizeof(*this));

    this->SetSourceTag("");
    this->SetEntryPointName("main");
    this->SetOptimizationProfile(VexShaderOptimizationProfile::MaxSpeed);
}

VexShaderCompiler& VexShaderCompiler::SetVertexSourceGLSL(const char* source, uint32_t size)
{
    this->info.vertexSource = source;
    this->info.vertexSourceSize = size;
    return *this;
}

VexShaderCompiler& VexShaderCompiler::SetFragSourceGLSL(const char* source, uint32_t size)
{
    this->info.fragSource = source;
    this->info.fragSourceSize = size;
    return *this;
}

VexShaderCompiler& VexShaderCompiler::SetSourceTag(const char* tag)
{
    this->info.sourceTag = tag;
    return *this;
}

VexShaderCompiler& VexShaderCompiler::SetEntryPointName(const char* name)
{
    this->info.entryPointName = name;
    return *this;
}

VexShaderCompiler& VexShaderCompiler::SetVertexKeywords(const VexShaderKeyword* keywords, uint32_t keywordCount)
{
    this->info.vertexKeywords = keywords;
    this->info.vertexKeywordCount = keywordCount;
    return *this;
}

VexShaderCompiler& VexShaderCompiler::SetFragKeywords(const VexShaderKeyword* keywords, uint32_t keywordCount)
{
    this->info.fragKeywords = keywords;
    this->info.fragKeywordCount = keywordCount;
    return *this;
}

VexShaderCompiler& VexShaderCompiler::SetOptimizationProfile(VexShaderOptimizationProfile profile)
{
    this->info.optimizationProfile = profile;
    return *this;
}

VexShaderCompilation VexShaderCompiler::Compile()
{
    return VexCompileShader(&this->info);
}