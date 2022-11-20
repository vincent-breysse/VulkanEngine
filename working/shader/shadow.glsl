
// COMMON
// ==============================================================================

#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_multiview : enable

const int MaxPointLightCount = 256;
const int MaxPointLightCastingShadows = 64;

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

layout (set = 1, binding = 0) uniform RenderPassUniforms0View
{
    mat4 WorldView;
    mat4 ViewClip;
    mat4 WorldClip;
    mat4 ClipWorld;
    vec4 ViewerPos4;
    uvec2 RenderTargetResolution;
};

layout (set = 1, binding = 1) uniform RenderPassUniforms1Shadow
{
    mat4 ShadowWorldClips[6 * MaxPointLightCastingShadows];
    vec4 ShadowParams[MaxPointLightCastingShadows];
};

layout (set = 1, binding = 2) uniform ShadowCubeUniforms
{
    uint ShadowCubeID;
};

layout(set = 1, binding = 3) buffer readonly EntityIDBuffer
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

layout (location = 0) out vec3 VertexPosition;

void main ()
{
    uint entityID = EntityIDs[gl_InstanceIndex];
    uint cubeFaceID = ShadowCubeID * 6 + gl_ViewIndex;
    
    mat4 localToWorld = LocalWorlds[entityID];
    mat4 shadowWorldClip = ShadowWorldClips[cubeFaceID];

    vec4 position4 = vec4(InputPositionLS, 1) * localToWorld;
    vec4 position4CS = position4 * shadowWorldClip;

    VertexPosition = position4.xyz;
    gl_Position = position4CS;
}

// FRAGMENT SHADER
// ==============================================================================
#elif FRAGMENT_SHADER

float GetShadowFar(uint i)
{
    return ShadowParams[i].y;
}

layout (location = 0) in vec3 VertexPosition;

void main () 
{ 
    float far = GetShadowFar(ShadowCubeID);
    vec3 lightPos = PointLightPosAndRanges[ShadowCubeID].xyz;

    gl_FragDepth = clamp(length(VertexPosition - lightPos) / far, 0, 1);
}

#endif