#pragma once

#include <cstdint>
#include <vulkan/vulkan.h>

struct VexReflectDescriptorSetBinding
{
    std::string typeName;
    std::string name;
    uint32_t set;
    uint32_t binding;
    uint32_t count;
    VkDescriptorType type;
    VkShaderStageFlags accessMask;
};

struct VexReflectDescriptorSet
{
    uint32_t set;
    uint32_t rangeBegin;
    uint32_t rangeEnd;
};

struct VexShaderStageCompilation
{
    std::vector<uint32_t> spirvCode;
    std::string errorMessage;
    VkShaderStageFlagBits stage;
    uint32_t spirvCodeSize;
    uint32_t errorCount;
    uint32_t warningCount;
};

struct VexShaderReflection
{
    std::vector<VexReflectDescriptorSetBinding> bindings;
    std::vector<VexReflectDescriptorSet> descriptorSets;
};

enum class VexShaderOptimizationProfile
{
    Null,
    None,
    MinSize,
    MaxSpeed
};

struct VexShaderKeyword
{
    const char* name;
    const char* value;
};

struct VexShaderCompilation
{
    VexShaderStageCompilation vertex;
    VexShaderStageCompilation frag;
    VexShaderReflection reflection;
};

struct VexShaderCompilationInput
{
    const char* vertexSource;
    size_t vertexSourceSize;
    const char* fragSource;
    size_t fragSourceSize;
    const char* sourceTag;
    const char* entryPointName;
    VexShaderOptimizationProfile optimizationProfile;
    const VexShaderKeyword* vertexKeywords;
    uint32_t vertexKeywordCount;
    const VexShaderKeyword* fragKeywords;
    uint32_t fragKeywordCount;
};

VexShaderCompilation VexCompileShader(const VexShaderCompilationInput* info);

struct VexShaderCompiler
{
    VexShaderCompiler();

    VexShaderCompiler& SetVertexSourceGLSL(const char* source, uint32_t size);

    VexShaderCompiler& SetFragSourceGLSL(const char* source, uint32_t size);

    VexShaderCompiler& SetSourceTag(const char* tag);

    VexShaderCompiler& SetEntryPointName(const char* name);

    VexShaderCompiler& SetVertexKeywords(const VexShaderKeyword* keywords, uint32_t keywordCount);

    VexShaderCompiler& SetFragKeywords(const VexShaderKeyword* keywords, uint32_t keywordCount);

    VexShaderCompiler& SetOptimizationProfile(VexShaderOptimizationProfile profile);

    VexShaderCompilation Compile();

    VexShaderCompilationInput info;
};