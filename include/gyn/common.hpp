#pragma once

#include <gyn/common/pch.hpp>
#include <gyn/common/fwd.hpp>
#include <gyn/common/config.hpp>
#include <gyn/common/error.hpp>
#include <gyn/common/misc.hpp>
#include <gyn/common/eastl.hpp>
#include <gyn/common/fmt.hpp>
#include <gyn/common/cast.hpp>
#include <gyn/common/atomic.hpp>

namespace Gyn
{
    struct AABB
    {
        AABB() = default;

        AABB(float3 center, float3 extents) : center(center), extents(extents) {}

        float3 center;
        float3 extents;
    };

    struct Sphere
    {
        Sphere() = default;

        Sphere(float3 center, float radius) : center(center), radius(radius) {}

        float3 center;
        float radius;
    };

    inline float Rand01()
    {
        return rand() / float(RAND_MAX);
    }

    inline  float RandNeg1Pos1()
    {
        return Rand01() * 2.0f - 1.0f;
    }

    inline  float3 RandV301()
    {
        return float3(Rand01(), Rand01(), Rand01());
    }

    inline Matrix4x4 Mat4ProjToVulkanProj(Matrix4x4 m)
    {
        // Reverse Y column since Vulkan has Y axis flipped
        m(0, 1) = -m(0, 1);
        m(1, 1) = -m(1, 1);
        m(2, 1) = -m(2, 1);
        m(3, 1) = -m(3, 1);

        return m;
    }

    inline float3 Mat4ExtractRight(const Matrix4x4& m) { return float3(m(0, 0), m(0, 1), m(0, 2)); }
    inline float3 Mat4ExtractLeft(const Matrix4x4& m) { return -Mat4ExtractRight(m); }

    inline float3 Mat4ExtractUp(const Matrix4x4& m) { return float3(m(1, 0), m(1, 1), m(1, 2)); }
    inline float3 Mat4ExtractDown(const Matrix4x4& m) { return -Mat4ExtractUp(m); }

    inline float3 Mat4ExtractForward(const Matrix4x4& m) { return float3(m(2, 0), m(2, 1), m(2, 2)); }
    inline float3 Mat4ExtractBackward(const Matrix4x4& m) { return -Mat4ExtractForward(m); }

    inline float3 Mat4ExtractTranslation(const Matrix4x4& m) { return float3(m(3, 0), m(3, 1), m(3, 2)); }

    struct TransformRecord
    {
        float sampleFrequency;
        ea::vector<float3> positions;
        ea::vector<Quaternion> rotations;
    };

    enum class EntityArchetypeID : uint64 {};
    constexpr EntityArchetypeID EntityArchetypeNullID = EntityArchetypeID(0);

    enum class EntityID : uint64 {};
    constexpr EntityID EntityNullID = EntityID(0);

    bool IsNull(EntityArchetypeID id);
    void AssertNotNull(EntityArchetypeID id);

    bool IsNull(EntityID id);
    void AssertNotNull(EntityID id);

    struct ECSComponentBase {};

    struct ECSComponent : public ECSComponentBase {};

    struct ECSAbstractComponent : public ECSComponentBase
    {
        virtual ~ECSAbstractComponent() = default;

    protected:

        ECSAbstractComponent() = default;

        ECSAbstractComponent(ECSAbstractComponent&&) = default;
        ECSAbstractComponent(const ECSAbstractComponent&) = default;

        ECSAbstractComponent& operator=(ECSAbstractComponent&&) = default;
        ECSAbstractComponent& operator=(const ECSAbstractComponent&) = default;
    };

    struct EntityMetadata
    {
        uint32 chunkLocalIndex;
        EntityArchetypeID archetypeID;
    };

    bool operator==(const EntityMetadata& a, const EntityMetadata& b);
    bool operator!=(const EntityMetadata& a, const EntityMetadata& b);

    struct ComponentTypeInfo
    {
        uint32 size;
        bool isAbstract;
    };

    bool operator==(const ComponentTypeInfo& a, const ComponentTypeInfo& b);
    bool operator!=(const ComponentTypeInfo& a, const ComponentTypeInfo& b);

    enum class ComponentLaneVersion : uint64 {};

    struct EntityIDComponent : ECSComponent
    {
        EntityIDComponent() = default;

        EntityIDComponent(EntityID id) : val(id) {}

        EntityID val = {};
    };

    void RegisterComponentType(TypeID id, const ComponentTypeInfo& info);

    ComponentTypeInfo GetComponentTypeInfo(TypeID id);

    uint32 GetComponentSize(TypeID id);

    bool IsComponentAbstract(TypeID id);

    template <typename T>
    void RegisterComponentType()
    {
        static_assert(std::is_base_of_v<ECSComponentBase, T>,
            "The Type T passed to RegisterComponentType<T> must inherit from ECSComponentBase");

        constexpr bool IsAbstract = std::is_base_of_v<ECSAbstractComponent, T>;
        if constexpr (!IsAbstract)
        {
            static_assert(std::is_trivially_copyable_v<T>,
                "If T is not a ECSAbstractComponent, it must be trivially copyable");

            static_assert(std::is_trivially_destructible_v<T>,
                "If T is not a ECSAbstractComponent, it must be trivially destructible");
        }

        ComponentTypeInfo info = {};
        info.size = sizeof(T);
        info.isAbstract = IsAbstract;

        RegisterComponentType(GetTypeID<T>(), info);
    }

    void RegisterAllComponents();

#define GYN_ECS_STATIC_ASSERT_INHERIT_FROM_COMPONENT_BASE(T) static_assert(::std::is_base_of_v<::Gyn::ECSComponentBase, T>, \
    #T " in function " GYN_FUNC " must inherit from ECSComponentBase")

#define GYN_ECS_STATIC_ASSERT_INHERIT_FROM_COMPONENT(T) static_assert(::std::is_base_of_v<::Gyn::ECSComponent, T>, \
    #T " in function " GYN_FUNC " must inherit from ECSComponent")

#define GYN_ECS_STATIC_ASSERT_INHERIT_FROM_ABSTRACT_COMPONENT(T) static_assert(::std::is_base_of_v<::Gyn::ECSAbstractComponent, T>, \
    #T " in function " GYN_FUNC " must inherit from ECSAbstractComponent")

#define GYN_ECS_STATIC_ASSERT_VALID_COMPONENT_BASE(T) \
GYN_ECS_STATIC_ASSERT_INHERIT_FROM_COMPONENT_BASE(T); \
GYN_STATIC_ASSERT_DEFAULT_CONSTRUCTIBLE(T)

#define GYN_ECS_STATIC_ASSERT_VALID_COMPONENT(T) \
GYN_ECS_STATIC_ASSERT_INHERIT_FROM_COMPONENT(T); \
GYN_STATIC_ASSERT_TRIVIALLY_COPYABLE(T); \
GYN_STATIC_ASSERT_DEFAULT_CONSTRUCTIBLE(T)

#define GYN_ECS_STATIC_ASSERT_VALID_ABSTRACT_COMPONENT(T) \
GYN_ECS_STATIC_ASSERT_INHERIT_FROM_ABSTRACT_COMPONENT(T); \
GYN_STATIC_ASSERT_DEFAULT_CONSTRUCTIBLE(T)

}