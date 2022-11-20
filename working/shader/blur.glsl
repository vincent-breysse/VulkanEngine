
// COMMON
// ==============================================================================

#version 450
#extension GL_ARB_separate_shader_objects : enable

const float PI = 3.14159265359f;
const int MaxPointLightCount = 256;

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
    uint Dummy10;
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

// Function used to generate GaussWeight
// Use the the standard normal distribution. (expectation = 0;  standard deviation = 1)
//
// float gauss(float x)
// {
//     return (1 / 2.507f) * exp(-0.5f * x * x);
// }

const int NbWeights = 5;
const float GaussWeights[] = float[NbWeights] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

layout (set = 3, binding = 0) uniform sampler2D Image2D;

layout (location = 0) in vec2 VertexUV;

layout (location = 0) out vec4 FragColor;

vec2 GetTexelGap()
{
    return 1.0 / textureSize(Image2D, 0);
}

vec3 SampleColor(vec2 uv)
{
    return texture(Image2D, uv).rgb;
}

vec3 ComputeHorizontalWeightedSum(vec2 uv)
{
    vec2 texelGap = GetTexelGap();
    vec3 result = GaussWeights[0] * SampleColor(uv);

    for (int i = 1; i < NbWeights; ++i)
    {
        vec2 offset = vec2(i * texelGap.x, 0);

        result += GaussWeights[i] * SampleColor(uv + offset);
        result += GaussWeights[i] * SampleColor(uv - offset);
    }

    return result;
}

vec3 ComputeSquareWeightedSum(vec2 uv)
{
    vec2 texelGap = GetTexelGap();
    vec3 result = GaussWeights[0] * ComputeHorizontalWeightedSum(uv);

    for (int i = 1; i < NbWeights; ++i)
    {
        vec2 offset = vec2(0, i * texelGap.y);

        result += GaussWeights[i] * ComputeHorizontalWeightedSum(uv + offset);
        result += GaussWeights[i] * ComputeHorizontalWeightedSum(uv - offset);
    }

    return result;
}

void main()
{
    FragColor = vec4(ComputeSquareWeightedSum(VertexUV), 1.0);
} 

#endif