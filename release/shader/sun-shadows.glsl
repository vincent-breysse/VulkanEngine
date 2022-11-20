
// COMMON
// ==============================================================================

#version 450
#extension GL_ARB_separate_shader_objects : enable

const int MaxPointLightCount = 256;

struct LitMaterialUniforms_t
{
    vec4 Dummy;
};

layout (set = 0, binding = 0) uniform GlobalUniforms
{
    mat4 SunShadowWorldClip;
    vec4 DebugFloat4;
    vec4 AmbientAndIntensity;
    vec4 DirLightDirection4;
    vec4 DirLightColorAndIntensity;

    vec4 PointLightPosAndRanges[MaxPointLightCount];
    vec4 PointLightColorAndIntensities[MaxPointLightCount];

    vec4 ColorFilterAndIntensity;
    vec4 BloomColorAndIntensity;
    float Contrast;
    float Saturation;
    float Exposure;

    uint PointLightCount;
    uint UseAO;
    uint DebugBuffer;
    uint UseEmissive;
    uint UseBloom;
    uint CastShadows;
};

layout(set = 0, binding = 1) buffer readonly ModelMatrixBuffer
{
    mat4 LocalWorlds[];
};

layout(set = 0, binding = 2) buffer readonly LitMaterialUniformsBuffer
{
    LitMaterialUniforms_t LitMaterialUniforms[];
};

layout(set = 1, binding = 0) buffer readonly EntityIDBuffer
{
    uint EntityIDs[];
};

layout(set = 2, binding = 0) uniform SubpassUniforms
{
    uint Dummy20;
};

layout (set = 3, binding = 0) uniform MaterialUniforms 
{
    uint Dummy30;
};

// VERTEX SHADER
// ==============================================================================
#if VERTEX_SHADER

layout (location = 0) in vec3 InputPositionLS;
layout (location = 1) in vec3 InputNormalLS;
layout (location = 2) in vec2 InputUV;
layout (location = 3) in vec3 InputTangent;

void main ()
{
    uint entityID = EntityIDs[gl_InstanceIndex];
    vec4 position4 = vec4(InputPositionLS, 1) * LocalWorlds[entityID];
    vec4 position4CS = position4 * SunShadowWorldClip;

    gl_Position = position4CS;
}

// FRAGMENT SHADER
// ==============================================================================
#elif FRAGMENT_SHADER

void main () 
{ 
}

#endif