#pragma once

#include <gyn/common.hpp>
#include <gyn/entity.hpp>

namespace Gyn
{
    struct Position : ECSComponent
    {
        Position() = default;

        Position(float3 pos) : val(pos) {}

        float3 val;
    };

    struct Rotation : ECSComponent
    {
        Rotation() = default;

        Rotation(Quaternion rotation) : val(rotation) {}

        Quaternion val;
    };

    struct Scale : ECSComponent
    {
        Scale() = default;

        Scale(float3 scale) : val(scale) {}

        float3 val;
    };

    struct LocalToWorld : ECSComponent
    {
        LocalToWorld() = default;

        LocalToWorld(const Matrix4x4& mat) : val(mat) {}

        Matrix4x4 val;
    };

    struct LocalRenderBounds : ECSComponent
    {
        LocalRenderBounds() = default;

        LocalRenderBounds(AABB aabb) : val(aabb) {}

        AABB val;
    };

    struct WorldRenderBounds : ECSComponent
    {
        WorldRenderBounds() = default;

        WorldRenderBounds(AABB aabb) : val(aabb) {}

        AABB val;
    };

    class Transform
    {
    public:

        Transform() = default;

        Transform(Entity entity);

        bool IsNull() const;

        void SetPosition(float3 position);

        float3 GetPosition() const;

        void Translate(float3 offset);

        void SetRotation(Quaternion rotation);

        Quaternion GetRotation() const;

        void Rotate(Quaternion rotation);

        void SetScale(float3 scale);

        float3 GetScale() const;

        void Scale(float3 scale);

        void SetLocalToWorld(const Matrix4x4& matrix);

        Matrix4x4 GetLocalToWorld() const;

        float3 Up() const;

        float3 Down() const;

        float3 Right() const;

        float3 Left() const;

        float3 Forward() const;

        float3 Backward() const;

    private:

        void AssertNotNull() const;

        Entity m_entity = {};
    };

    void UpdateLocalToWorlds(World& world);

    void UpdateWorldRenderBounds(World& world);

    void UpdateWorldToView(World& world);

    void UpdateViewToClip(World& world);

    void UpdateWorldToClip(World& world);
}