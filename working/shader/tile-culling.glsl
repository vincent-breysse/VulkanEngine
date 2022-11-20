
// COMMON
// ==============================================================================

#version 450
#extension GL_ARB_separate_shader_objects : enable

const float PI = 3.14159265359f;
const int MaxPointLightCount = 256;
const int MaxLightPerTile = 64;

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
    uint Dummy01;
};

layout(set = 0, binding = 2) buffer readonly LitMaterialUniformsBuffer
{
    uint Dummy02;
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
    uint Dummy11;
};

layout (set = 2, binding = 0) uniform SubpassUniforms 
{
    uint Dummy20;
};

// VERTEX SHADER
// ==============================================================================
#if VERTEX_SHADER

layout (location = 0) in vec3 InputPositionLS;
layout (location = 1) in vec3 InputNormalLS;
layout (location = 2) in vec2 InputUV;
layout (location = 3) in vec3 InputTangentLS;

layout (location = 0) out vec2 VertexUV;

void main ()
{
    // Need to reverse Y since Vulkan Y axis point downward
    float y = -InputPositionLS.y;
    vec4 positionCS = vec4(vec3(InputPositionLS.x, y, InputPositionLS.z), 1);

    VertexUV = InputUV;
    gl_Position = positionCS;
}

// FRAGMENT SHADER
// ==============================================================================
#elif FRAGMENT_SHADER

layout (set = 3, binding = 0) uniform sampler2D DepthNSBuffer;
layout (set = 3, binding = 1) buffer TileCullingBufferOutput
{
    uint PointLightIndices[];
};

layout (location = 0) in vec2 VertexUV;

layout (location = 0) out uint FragPointLightCount;

vec3 ComputeFragPosition(vec2 uv, ivec2 pixelCoord)
{
    vec2 xyNS = uv * 2 - vec2(1, 1);
    float zNS = texelFetch(DepthNSBuffer, pixelCoord, 0).x;
    vec3 posNS = vec3(xyNS, zNS);

    vec4 unproj4 = vec4(posNS, 1) * ClipWorld;
    vec3 posWS = unproj4.xyz / unproj4.w;

    return posWS;
}

bool AABBOverlapSphere(vec3 minAABB, vec3 maxAABB, vec3 center, float radius)
{
    // SIMD version of this algorithm
    // -------------------------------------
    // float r2 = radius * radius;
    // float dmin = 0;
    // for (int i = 0; i < 3; i++)
    // {
    //     if (center[i] < minAABB[i])
    //     {
    //         float d = center[i] - minAABB[i];
    //         dmin += d * d;
    //     }
    //     else if (center[i] > maxAABB[i])
    //     {
    //         float d = center[i] - maxAABB[i];
    //         dmin += d * d;
    //     }
    // }
    // return dmin <= r2;

    vec3 d0 = center - minAABB;
    vec3 d1 = center - maxAABB;

    bvec3 b0 = lessThan(center, minAABB);
    bvec3 b1 = greaterThan(center, maxAABB);

    vec3 dminv = vec3(b0) * d0 + vec3(b1) * d1;
    dminv *= dminv;

    float r2 = radius * radius;
    float dmin = dminv.x + dminv.y + dminv.z;

    return dmin <= r2;
}

void main()
{
    uvec2 tileCoord = uvec2(floor(gl_FragCoord.xy));
    uvec2 tileSize = uvec2(ceil(vec2(RenderTargetResolution) / vec2(TileCullingResolution)));
    uvec2 fragOffset = tileCoord * tileSize;

    vec3 minAABB = vec3(1,1,1) * 1e9;
    vec3 maxAABB = vec3(1,1,1) * -1e9;

    for (int y = 0; y < tileSize.y; ++y)
    {
        for (int x = 0; x < tileSize.x; ++x)
        {
            uvec2 fragCoord = fragOffset + uvec2(x, y);
            vec2 fragUV = vec2(fragCoord) / vec2(RenderTargetResolution);
            vec3 fragPos = ComputeFragPosition(fragUV, ivec2(fragCoord));

            minAABB = min(minAABB, fragPos);
            maxAABB = max(maxAABB, fragPos);
        }
    }

    uint flatTileIndex = tileCoord.y * TileCullingResolution.x + tileCoord.x;
    uint bufferOutputOffset = flatTileIndex * MaxLightPerTile;

    uint lightCount = 0;

    for (int i = 0; i < PointLightCount && lightCount < MaxLightPerTile; ++i)
    {
        vec4 posAndRange = PointLightPosAndRanges[i];
        vec3 pos = posAndRange.xyz;
        float range = posAndRange.w;

        if (AABBOverlapSphere(minAABB, maxAABB, pos, range))
        {
            PointLightIndices[bufferOutputOffset + lightCount] = i;
            ++lightCount;
        }
    }

    FragPointLightCount = lightCount;
} 

#endif