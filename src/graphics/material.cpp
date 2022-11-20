
#include <pch.hpp>
#include <gyn/graphics/material.hpp>
#include <gyn/graphics/image2d.hpp>

static MaterialID CreateUniqueMaterialID()
{
    static std::atomic<uint64> id = 1;
    return MaterialID(id++);
}

Material::Material(GPU& gpu, const GPUShader* shader, TypeID uniformStructTypeID, uint64 uniformStructSizeByte)
{
    ASSERT(shader);
    ASSERT_MSG(uniformStructSizeByte > 0,
        "Unless MaterialNoUniforms struct is specified to create a material, uniformStructSizeByte must be greater than 0");

    const bool isNoUniform = uniformStructTypeID == GetTypeID<MaterialNoUniforms>();
    if (isNoUniform)
    {
        ASSERT(uniformStructSizeByte == sizeof(MaterialNoUniforms));
    }

    const MaterialID id = CreateUniqueMaterialID();
    UniqueGPUBindSetPtr bindSet = gpu.CreateBindSet(*shader, 3);

    ea::vector<byte> uniformData;
    if (!isNoUniform)
    {
        uniformData = ea::vector<byte>(uniformStructSizeByte);
    }

    m_id = id;
    m_bindSet = std::move(bindSet);
    m_shader = shader;
    m_uniformData = std::move(uniformData);
    m_uniformSructType = uniformStructTypeID;
}

bool Material::IsNull() const
{
    return m_id == MaterialNullID;
}

void Material::AddOwnedBuffer(UniqueGPUBufferPtr&& buffer)
{
    AssertNotNull();
    m_ownedBuffers.push_back(std::move(buffer));
}

void Material::AddOwnedImage(SharedPtr<const Image2D>&& image)
{
    AssertNotNull();
    m_ownedImages.push_back(std::move(image));
}

void Material::SetBinding(const GPUBuffer* buffer, uint32 binding)
{
    AssertNotNull();
    m_bindSet->SetBinding(buffer, binding);
}

void Material::SetBinding(const GPUImageView* imageView, uint32 binding, GPUSamplerFiltering filtering, GPUSamplerAddressing addressing)
{
    AssertNotNull();
    m_bindSet->SetBinding(imageView, binding, filtering, addressing);
}

MaterialID Material::GetID() const
{
    AssertNotNull();
    return m_id;
}

void Material::AssertNotNull() const
{
    ASSERT(!IsNull());
}

void Material::AssertHasUniformData() const
{
    AssertNotNull();
    ASSERT_MSG(HasUniformData(),
        "The Material has been declared without uniform data");
}

void Material::AssertValidType(TypeID typeID, usize typeSize) const
{
    AssertNotNull();
    AssertHasUniformData();
    ASSERT_MSG(typeID == m_uniformSructType,
        "The Material has been created to use another uniform struct type");
    ASSERT_MSG(typeSize == m_uniformData.size(), 
        "Buffer size mismatch");
}

GPU& Material::GetGPU()
{
    AssertNotNull();
    return m_bindSet->GetGPU();
}

const GPUShader* Material::GetShader() const
{
    AssertNotNull();
    return m_shader;
}

const GPUBindSet* Material::GetBindSet() const
{
    AssertNotNull();
    return m_bindSet.get();
}

bool Material::HasUniformData() const
{
    AssertNotNull();
    return !m_uniformData.empty();
}

void Material::SetUniformData(ea::span<const byte> data, TypeID uniformStructType)
{
    AssertNotNull();
    AssertHasUniformData();
    AssertValidType(uniformStructType, data.size());

    memcpy(m_uniformData.data(), data.data(), data.size());
    ++m_uniformDataVersion;
}

ea::span<const byte> Material::GetUniformData() const
{
    AssertNotNull();
    AssertHasUniformData();
    return m_uniformData;
}

uint32 Material::GetUniformDataVersion() const
{
    AssertNotNull();
    AssertHasUniformData();
    return m_uniformDataVersion;
}

TypeID Material::GetUniformDataType() const
{
    AssertNotNull();
    AssertHasUniformData();
    return m_uniformSructType;
}

SharedPtr<Material> Gyn::CreateMaterial(GPU& gpu, const GPUShader* shader, TypeID uniformStructTypeID, uint64 uniformStructSizeByte)
{
    return ea::make_shared<Material>(gpu, shader, uniformStructTypeID, uniformStructSizeByte);
}