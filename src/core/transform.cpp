
#include <pch.hpp>
#include <gyn/world.hpp>
#include <gyn/entity_archetype.hpp>
#include <gyn/transform.hpp>

Transform::Transform(Entity entity)
{
    const EntityArchetype archetype = entity.GetArchetype();

    const ea::array RequiredTypes
    {
        GetTypeID<Position>(),
        GetTypeID<Rotation>(),
        GetTypeID<::Scale>(),
        GetTypeID<LocalToWorld>(),
    };

    ASSERT(archetype.HasAllTypes(RequiredTypes));

    m_entity = entity;
}

bool Transform::IsNull() const
{
    return m_entity.IsNull();
}

void Transform::SetPosition(float3 position)
{
    AssertNotNull();
    Position t{};
    t.val = position;
    m_entity.SetComponent<Position>(t);
}

float3 Transform::GetPosition() const
{
    AssertNotNull();
    return m_entity.GetComponent<Position>().val;
}

void Transform::Translate(float3 offset)
{
    AssertNotNull();
    SetPosition(GetPosition() + offset);
}

void Transform::SetRotation(Quaternion rotation)
{
    AssertNotNull();

    Rotation t{};
    t.val = rotation;
    m_entity.SetComponent<Rotation>(t);
}

Quaternion Transform::GetRotation() const
{
    AssertNotNull();
    return m_entity.GetComponent<Rotation>().val;
}

void Transform::Rotate(Quaternion rotation)
{
    AssertNotNull();
    SetRotation(GetRotation() * rotation);
}

void Transform::SetScale(float3 scale)
{
    AssertNotNull();
    ::Scale t{};
    t.val = scale;
    m_entity.SetComponent<::Scale>(t);
}

float3 Transform::GetScale() const
{
    AssertNotNull();
    return m_entity.GetComponent<::Scale>().val;
}

void Transform::Scale(float3 scale)
{
    AssertNotNull();
    SetScale(GetScale() * scale);
}

void Transform::SetLocalToWorld(const Matrix4x4& matrix)
{
    AssertNotNull();
    m_entity.SetComponent(LocalToWorld{ matrix });
}

Matrix4x4 Transform::GetLocalToWorld() const
{
    AssertNotNull();
    return Matrix4x4(m_entity.GetComponent<LocalToWorld>().val);
}

float3 Transform::Up() const
{
    AssertNotNull();
    return Mat4ExtractUp(GetLocalToWorld());
}

float3 Transform::Down() const
{
    AssertNotNull();
    return Mat4ExtractDown(GetLocalToWorld());
}

float3 Transform::Right() const
{
    return Mat4ExtractRight(GetLocalToWorld());
}

float3 Transform::Left() const
{
    AssertNotNull();
    return Mat4ExtractLeft(GetLocalToWorld());
}

float3 Transform::Forward() const
{
    return Mat4ExtractForward(GetLocalToWorld());
}

float3 Transform::Backward() const
{
    return Mat4ExtractBackward(GetLocalToWorld());
}

void Transform::AssertNotNull() const
{
    ASSERT(!IsNull());
}

static Matrix4x4 ComputeTRS(Position position, Rotation rotation, Scale scale)
{
    return Mat4Scaling(scale.val) * Mat4RotationQuaternion(rotation.val) * Mat4Translation(position.val);
}

static void UpdateLocalToWorlds(ea::vector<EntityChunk*>&& chunks)
{
    struct Versions
    {
        ComponentLaneVersion position{};
        ComponentLaneVersion rotation{};
        ComponentLaneVersion scale{};
    };
    static ea::hash_map<EntityArchetypeID, Versions> versionMap{};

    for (EntityChunk* chunk : chunks)
    {
        Versions& version = versionMap[chunk->GetArchetype().GetID()];

        ComponentLaneVersion newPositionVersion = chunk->GetLaneVersion<Position>();
        ComponentLaneVersion newRotationVersion = chunk->GetLaneVersion<Rotation>();
        ComponentLaneVersion newScaleVersion = chunk->GetLaneVersion<Scale>();

        if (version.position != newPositionVersion || version.rotation != newRotationVersion || version.scale != newScaleVersion)
        {
            uint64 count = chunk->GetEntityCount();
            const Position* positions = chunk->GetComponentLane<Position>();
            const Rotation* rotations = chunk->GetComponentLane<Rotation>();
            const Scale* scales = chunk->GetComponentLane<Scale>();
            LocalToWorld* localToWorlds = chunk->GetMutableComponentLane<LocalToWorld>();

            for (int k = 0; k < count; ++k)
            {
                const Position& position = positions[k];
                const Rotation& rotation = rotations[k];
                const Scale& scale = scales[k];
                LocalToWorld& localToWorld = localToWorlds[k];

                localToWorld = ComputeTRS(position, rotation, scale);
            }

            version.position = newPositionVersion;
            version.rotation = newRotationVersion;
            version.scale = newScaleVersion;
        }
    }
}

void Gyn::UpdateLocalToWorlds(World& world)
{
    const ea::array AllTypes =
    {
        GetTypeID<Position>(),
        GetTypeID<Rotation>(),
        GetTypeID<Scale>(),
        GetTypeID<LocalToWorld>(),
    };

    ::UpdateLocalToWorlds(world.entityManager.QueryEntityChunks(AllTypes));
}

static AABB AABBTransform(const AABB& aabb, const Matrix4x4& matrix)
{
    static const vfloat4 BoxOffsets[8] =
    {
        v4Set(-1.0f, -1.0f,  1.0f, 0.0f),
        v4Set(1.0f, -1.0f,  1.0f, 0.0f),
        v4Set(1.0f,  1.0f,  1.0f, 0.0f),
        v4Set(-1.0f,  1.0f,  1.0f, 0.0f),
        v4Set(-1.0f, -1.0f, -1.0f, 0.0f),
        v4Set(1.0f, -1.0f, -1.0f, 0.0f),
        v4Set(1.0f,  1.0f, -1.0f, 0.0f),
        v4Set(-1.0f,  1.0f, -1.0f, 0.0f),
    };

    vfloat4x4 m = vLoad(&matrix);

    // Load center and extents.
    vfloat4 vCenter = vLoad(&aabb.center);
    vfloat4 vExtents = vLoad(&aabb.extents);

    // Compute and transform the corners and find new min/max bounds.
    vfloat4 Corner = v4MulAdd(vExtents, BoxOffsets[0], vCenter);
    Corner = v3TransformPoint(Corner, m);

    vfloat4 Min, Max;
    Min = Max = Corner;

    for (size_t i = 1; i < 8; ++i)
    {
        Corner = v4MulAdd(vExtents, BoxOffsets[i], vCenter);
        Corner = v3TransformPoint(Corner, m);

        Min = v4Min(Min, Corner);
        Max = v4Max(Max, Corner);
    }

    // Store center and extents.
    AABB out;
    vStore(&out.center, v4Mul(v4Add(Min, Max), v4Spread(0.5f)));
    vStore(&out.extents, v4Mul(v4Sub(Max, Min), v4Spread(0.5f)));

    return out;
}

void Gyn::UpdateWorldRenderBounds(World& world)
{
    const ea::array AllTypes =
    {
        GetTypeID<LocalRenderBounds>(),
        GetTypeID<WorldRenderBounds>(),
        GetTypeID<LocalToWorld>(),
    };

    ea::vector<EntityChunk*> chunks = world.entityManager.QueryEntityChunks(AllTypes);

    uint32 begin = 0;
    for (EntityChunk* chunk : chunks)
    {
        uint32 count = chunk->GetEntityCount();
        const LocalToWorld* matrixLane = chunk->GetComponentLane<LocalToWorld>();
        const LocalRenderBounds* localBoundsLane = chunk->GetComponentLane<LocalRenderBounds>();
        WorldRenderBounds* worldBoundsLane = chunk->GetMutableComponentLane<WorldRenderBounds>();

        for (uint32 i = 0; i < count; ++i)
        {
            const Matrix4x4& matrix = matrixLane[i].val;
            const AABB& localBounds = localBoundsLane[i].val;
            AABB& worldBounds = worldBoundsLane[i].val;

            worldBounds = AABBTransform(localBounds, matrix);
        }

        begin += count;
    }
}

void Gyn::UpdateWorldToView(World& world)
{
    const ea::array AllTypes =
    {
        GetTypeID<Position>(),
        GetTypeID<Rotation>(),
        GetTypeID<WorldToView>(),
    };

    ea::vector<EntityChunk*> chunks = world.entityManager.QueryEntityChunks(AllTypes);

    for (EntityChunk* chunk : chunks)
    {
        const Position* positions = chunk->GetComponentLane<Position>();
        const Rotation* rotations = chunk->GetComponentLane<Rotation>();
        WorldToView* worldViews = chunk->GetMutableComponentLane<WorldToView>();

        for (uint32 i = 0; i < chunk->GetEntityCount(); ++i)
        {
            float3 pos = -positions[i].val;
            Quaternion rotation = rotations[i].val;
            rotation = QuatConjugate(rotation);

            worldViews[i].val = Mat4Translation(pos) * Mat4RotationQuaternion(rotation);
        }
    }
}

void Gyn::UpdateViewToClip(World& world)
{
    const ea::array AllTypes =
    {
        GetTypeID<Resolution>(),
        GetTypeID<PerspectiveProjection>(),
        GetTypeID<ViewToClip>(),
    };

    ea::vector<EntityChunk*> chunks = world.entityManager.QueryEntityChunks(AllTypes);

    for (EntityChunk* chunk : chunks)
    {
        const Resolution* resolutions = chunk->GetComponentLane<Resolution>();
        const PerspectiveProjection* projs = chunk->GetComponentLane<PerspectiveProjection>();
        ViewToClip* viewToClips = chunk->GetMutableComponentLane<ViewToClip>();

        for (uint32 i = 0; i < chunk->GetEntityCount(); ++i)
        {
            const PerspectiveProjection& proj = projs[i];
            uint2 resolution = resolutions[i].val;
            Matrix4x4& viewToClip = viewToClips[i].val;

            float aspectRatio = resolution.x / (float)resolution.y;
            Matrix4x4 projMatrix = Mat4PerspectiveFOV(DegToRad(proj.fov), aspectRatio, proj.near, proj.far);

            viewToClip = projMatrix;
        }
    }
}

void Gyn::UpdateWorldToClip(World& world)
{
    const ea::array AllTypes =
    {
        GetTypeID<WorldToView>(),
        GetTypeID<ViewToClip>(),
        GetTypeID<WorldToClip>(),
    };

    ea::vector<EntityChunk*> chunks = world.entityManager.QueryEntityChunks(AllTypes);

    for (EntityChunk* chunk : chunks)
    {
        const WorldToView* worldToViews = chunk->GetComponentLane<WorldToView>();
        const ViewToClip* viewToClips = chunk->GetComponentLane<ViewToClip>();
        WorldToClip* worldToClips = chunk->GetMutableComponentLane<WorldToClip>();

        for (uint32 i = 0; i < chunk->GetEntityCount(); ++i)
        {
            const Matrix4x4& worldToView = worldToViews[i].val;
            const Matrix4x4& viewToClip = viewToClips[i].val;
            Matrix4x4& worldToClip = worldToClips[i].val;

            worldToClip = worldToView * viewToClip;
        }
    }
}