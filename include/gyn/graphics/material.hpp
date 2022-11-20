#pragma once

#include <gyn/graphics/common.hpp>

namespace Gyn
{
    class Material : NoCopyMove
    {
    public:

        Material(GPU& gpu, const GPUShader* shader, TypeID uniformStructTypeID, uint64 uniformStructSizeByte);

        bool IsNull() const;

        void AddOwnedBuffer(UniqueGPUBufferPtr&& buffer);

        void AddOwnedImage(SharedPtr<const Image2D>&& image);

        void SetBinding(const GPUBuffer* buffer, uint32 binding);

        void SetBinding(const GPUImageView* imageView, uint32 binding,
            GPUSamplerFiltering filtering = GPUSamplerFiltering::eLinear,
            GPUSamplerAddressing addressing = GPUSamplerAddressing::eRepeat);

        MaterialID GetID() const;

        GPU& GetGPU();

        const GPUShader* GetShader() const;

        const GPUBindSet* GetBindSet() const;

        bool HasUniformData() const;

        void SetUniformData(ea::span<const byte> data, TypeID uniformStructType);

        template <typename T>
        void SetUniformData(const T& data);

        ea::span<const byte> GetUniformData() const;

        template <typename T>
        const T& GetUniformData() const;

        uint32 GetUniformDataVersion() const;

        TypeID GetUniformDataType() const;

        template <typename T>
        bool IsUniformDataType() const;

    private:

        void AssertNotNull() const;

        void AssertHasUniformData() const;

        void AssertValidType(TypeID typeID, usize typeSize) const;

        uint32 m_uniformDataVersion = {};
        TypeID m_uniformSructType = {};
        ea::vector<byte> m_uniformData = {};

        ea::vector<UniqueGPUBufferPtr> m_ownedBuffers = {};
        ea::vector<SharedPtr<const Image2D>> m_ownedImages = {};
        const GPUShader* m_shader = {};
        UniqueGPUBindSetPtr m_bindSet = {};
        MaterialID m_id = {};
    };

    template <typename T>
    void Material::SetUniformData(const T& data)
    {
        SetUniformData(AsByteSpan(data), GetTypeID<T>());
    }

    SharedPtr<Material> CreateMaterial(GPU& gpu, const GPUShader* shader, TypeID uniformStructTypeID, uint64 uniformStructSizeByte);

    template <typename T>
    SharedPtr<Material> CreateMaterial(GPU& gpu, const GPUShader* shader)
    {
        GYN_STATIC_ASSERT_TRIVIALLY_COPYABLE(T);
        return CreateMaterial(gpu, shader, GetTypeID<T>(), sizeof(T));
    }

    template <typename T>
    const T& Material::GetUniformData() const
    {
        AssertValidType(GetTypeID<T>(), sizeof(T));
        const ea::span<const byte> span = GetUniformData();
        return reinterpret_cast<const T&>(*span.data());
    }

    template <typename T>
    bool Material::IsUniformDataType() const
    {
        return GetUniformDataType() == GetTypeID<T>();
    }
}