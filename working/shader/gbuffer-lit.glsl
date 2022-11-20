
// COMMON
// ==============================================================================

#version 450
#extension GL_ARB_separate_shader_objects : enable

const int MaxPointLightCount = 256;

struct LitMaterialUniforms_t
{
    vec4 color;
    vec4 emissionColor;
    vec2 uvScale;
    float roughness;
    float metallic;
    float normal;
    float ao;
    int isUnlit;
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

layout (set = 1, binding = 0) uniform RenderPassUniforms
{
    mat4 WorldView;
    mat4 ViewClip;
    mat4 WorldClip;
    mat4 ClipWorld;
    vec4 ViewerPos4;
    uvec2 RenderTargetResolution;
    uvec2 TileCullingResolution;
};

layout(set = 1, binding = 1) buffer readonly EntityIDBuffer
{
    uint EntityIDs[];
};

layout (set = 2, binding = 0) uniform SubpassUniforms 
{
    uint Dummy0;
};

// VERTEX SHADER
// ==============================================================================
#if VERTEX_SHADER

layout (location = 0) in vec3 InputPositionLS;
layout (location = 1) in vec3 InputNormalLS;
layout (location = 2) in vec2 InputUV;
layout (location = 3) in vec3 InputTangentLS;

layout (location = 0) out mat3 VertexTangentWorld;
layout (location = 3) out vec3 VertexPosition;
layout (location = 4) out vec3 VertexNormal;
layout (location = 5) out vec2 VertexUV;

mat3 CreateTangentWorldMatrix(mat3 localWorldRotationT)
{
    vec3 bitangentLS = cross(InputTangentLS, InputNormalLS);

    vec3 a = InputTangentLS;
    vec3 b = bitangentLS;
    vec3 c = InputNormalLS;

    mat3 tangentLocal = mat3(a, b, c);

    return transpose(localWorldRotationT * tangentLocal);
}

void main ()
{
    uint entityID = EntityIDs[gl_InstanceIndex];
    
    mat4 localWorld = LocalWorlds[entityID];
    mat3 localWorldRotationT = inverse(mat3(localWorld));
    mat3 tangentWorld = CreateTangentWorldMatrix(localWorldRotationT);

    vec3 position = vec3(vec4(InputPositionLS, 1) * localWorld);
    vec3 normal = normalize(localWorldRotationT * InputNormalLS);
    vec4 position4CS = vec4(position, 1) * WorldClip;


    VertexUV = InputUV;
    VertexTangentWorld = tangentWorld;
    VertexPosition = position;
    VertexNormal = normal;
    gl_Position = position4CS;
}

// FRAGMENT SHADER
// ==============================================================================
#elif FRAGMENT_SHADER

layout (set = 3, binding = 0) uniform MaterialUniforms
{
    uint MaterialHeapIndex;
};

layout (set = 3, binding = 1) uniform sampler2D Albedo;
layout (set = 3, binding = 2) uniform sampler2D NormalMap;
layout (set = 3, binding = 3) uniform sampler2D EmissiveMap;
layout (set = 3, binding = 4) uniform sampler2D MaskMap;

layout (location = 0) in mat3 VertexTangentWorld;
layout (location = 3) in vec3 VertexPosition;
layout (location = 4) in vec3 VertexNormal;
layout (location = 5) in vec2 VertexUV;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 FragNormal;
layout (location = 2) out vec4 FragEmissive;
layout (location = 3) out vec4 FragMask;
layout (location = 4) out uint FragMaterialHeapIndex;

vec3 GetNormalTS(vec2 uv)
{
    return texture(NormalMap, uv).xyz * 2.f - 1;
}

void main () 
{ 
    LitMaterialUniforms_t matUniforms = LitMaterialUniforms[MaterialHeapIndex];
    vec2 uv = VertexUV * matUniforms.uvScale;

    vec3 albedo = matUniforms.color.rgb * texture(Albedo, uv).rgb;

    vec3 normalTS = GetNormalTS(uv);
    vec3 normalFromVertex = VertexNormal;
    vec3 normalFromTexture = normalize(normalTS * VertexTangentWorld);
    vec3 normal = mix(normalFromVertex, normalFromTexture, matUniforms.normal);

    vec3 emissive = matUniforms.emissionColor.rgb * texture(EmissiveMap, uv).rgb;

    vec4 maskTexel = texture(MaskMap, uv);
    float metallicTexel = maskTexel.x;
    float aoTexel = maskTexel.y;
    float detailsTexel = maskTexel.z;
    float smoothnessTexel = maskTexel.w;

    float metallic = metallicTexel * matUniforms.metallic;
    float ao = matUniforms.ao * aoTexel;
    float details = detailsTexel;
    float smoothness = (1.0 - matUniforms.roughness) * smoothnessTexel;

    FragColor = vec4(albedo, 1);
    FragNormal = vec4(normal, 0);
    FragEmissive = vec4(emissive, 1);
    FragMask = vec4(metallic, ao, details, smoothness);
    FragMaterialHeapIndex = MaterialHeapIndex;
}

#endif