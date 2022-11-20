
// COMMON
// ==============================================================================

#version 450
#extension GL_ARB_separate_shader_objects : enable

const float PI = 3.14159265359f;
const int SSAOKernelSize = 64;

layout (set = 0, binding = 0) uniform GlobalUniforms
{
    uint Dummy00;
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

layout (set = 3, binding = 0) uniform SSAOUniforms 
{
    vec4 Kernel[SSAOKernelSize];
    float SSAORadius;
    float SSAOStrength;
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

layout (set = 3, binding = 1) uniform sampler2D NoiseMap;
layout (set = 3, binding = 2) uniform sampler2D DepthNSBuffer;
layout (set = 3, binding = 3) uniform sampler2D NormalBuffer;

layout (location = 0) in vec2 VertexUV;

layout (location = 0) out float FragAO;

vec3 ComputeFragPosition(vec2 uv)
{
    vec2 xyNS = uv * 2 - vec2(1, 1);
    float zNS = texture(DepthNSBuffer, uv).x;
    vec3 posNS = vec3(xyNS, zNS);

    vec4 unproj4 = vec4(posNS, 1) * ClipWorld;
    vec3 posWS = unproj4.xyz / unproj4.w;

    return posWS;
}

vec3 ComputePositionVS(vec2 uv)
{
    vec3 position = ComputeFragPosition(uv);
    vec4 position4VS = vec4(position, 1) * WorldView;
    return position4VS.xyz;
}

vec3 ComputeNormalVS(vec2 uv)
{
    vec3 normal = texture(NormalBuffer, uv).xyz;
    vec3 normalVS = normalize(normal * mat3(WorldView));
    return normalVS;
}

vec3 GetSSAONoise(vec2 uv)
{
    vec2 scale = RenderTargetResolution / vec2(textureSize(NoiseMap, 0));
    vec2 noise01 = texture(NoiseMap, uv * scale).xy;
    vec2 noise = noise01 * 2 - vec2(1, 1);
    return normalize(vec3(noise, 0));
}

mat3 CreateTangentViewMatrixT(vec3 normalVS, vec3 noise)
{
    noise = normalize(noise);
    //vec3 tangentVS = normalize(noise - normalVS * dot(noise, normalVS));
    vec3 bitangentVS = normalize(cross(normalVS, noise));
    vec3 tangentVS = normalize(cross(normalVS, bitangentVS));
    
    vec3 a = tangentVS;
    vec3 b = bitangentVS;
    vec3 c = normalVS;

    return mat3(a, b, c);
}

vec3 VSToNS(vec3 posVS)
{
    vec4 posCS = vec4(posVS, 1) * ViewClip;
    vec3 posNS = posCS.xyz / posCS.w;
    return posNS;
}

vec2 GetUVFromNS(vec3 posNS)
{
    return posNS.xy * 0.5f + vec2(0.5f);
}

// Hermit interpolation
float HermitMix(float p0, float m0, float p1, float m1, float t)
{
    float t2 = t * t;
    float t3 = t2 * t;
    float _2t3 = 2.f * t3;
    float _3t2 = 3.f * t2;
    float _2t2 = 2.f * t2;

    return (_2t3 - _3t2 + 1.f) * p0
    + (t3 - _2t2 + t) * m0
    + (-_2t3 + _3t2) * p1
    + (t3 - t2) * m1;
}

vec2 HermitMix(vec2 p0, vec2 m0, vec2 p1, vec2 m1, float t)
{
    return vec2
    (
        HermitMix(p0.x, m0.x, p1.x, m1.x, t),
        HermitMix(p0.y, m0.y, p1.y, m1.y, t)
    );
}

vec3 HermitMix(vec3 p0, vec3 m0, vec3 p1, vec3 m1, float t)
{
    return vec3
    (
        HermitMix(p0.xy, m0.xy, p1.xy, m1.xy, t),
        HermitMix(p0.z, m0.z, p1.z, m1.z, t)
    );
}

float HemitCurve01(float t, float curve)
{
    return HermitMix(0, 0, 1, curve, t);
}

float HemitCurve01(float t)
{
    return HemitCurve01(t, 1.0);
}

void main()
{    
    vec3 fragPosVS = ComputePositionVS(VertexUV);
    vec3 fragNormalVS = ComputeNormalVS(VertexUV);
    vec3 noise = GetSSAONoise(VertexUV);
    mat3 tangentViewT = CreateTangentViewMatrixT(fragNormalVS, noise);
    float depthNS = texture(DepthNSBuffer, VertexUV).x;

    float wantedSamplingRadius = max(SSAORadius, 1e-6);
    float strength = SSAOStrength;

    const float MaxUncertaintyDistance = 100;
    const float MinUncertaintyRadius = 0.01; // Gives nicely detailled SSAO, but only works for close objects. Otherwise it tends to cause a lot of flickering.
    const float MaxUncertaintyRadius = 1.0;
    const float DeltaZBias = 0.001;
    
    float ambientOcclusion = 0.0f;
    if (depthNS < 1.0)
    {
        float maxSamplingRadius = max(wantedSamplingRadius, 5.0);

        float uncertaintyFactor = clamp(fragPosVS.z / MaxUncertaintyDistance, 0, 1);
        uncertaintyFactor = HemitCurve01(uncertaintyFactor, 2.0);

        float samplingRadius = mix(wantedSamplingRadius, maxSamplingRadius, uncertaintyFactor);

        for (int i = 0; i < SSAOKernelSize; ++i)
        {
            vec3 samplePosVS = fragPosVS + (tangentViewT * Kernel[i].xyz) * samplingRadius;
            vec3 samplePosNS = VSToNS(samplePosVS);

            vec2 uvSample = GetUVFromNS(samplePosNS);
            float sampleDepthVS = length(ComputePositionVS(uvSample));

            float signedDeltaZ = sampleDepthVS - length(samplePosVS);
            float deltaZ = abs(signedDeltaZ);

            float radiusFactor = clamp(deltaZ / samplingRadius, 0, 1);
            radiusFactor = 1.0 - HemitCurve01(radiusFactor);

            float uncertaintyRadius = mix(MinUncertaintyRadius, MaxUncertaintyRadius, uncertaintyFactor);

            // Lower impact of samples very close to the geometry to avoid artifacts
            float poximityFactor = clamp(deltaZ / uncertaintyRadius, 0, 1); 
            poximityFactor = HemitCurve01(poximityFactor, 4.0);

            // Lower impact of samples overflowing the texture to avoid artifacts
            vec3 overflowVector = abs(vec3(samplePosNS.xy, samplePosNS.z * 2.0 - 1.0)) - vec3(1.0);

            float maxOverflow = max(max(overflowVector.x, overflowVector.y), overflowVector.z);
            float overflowFactor = 1.0 - clamp(maxOverflow / 0.001, 0, 1);

            float isBiased = float(deltaZ >= DeltaZBias);
            float isOccluded = float(signedDeltaZ < 0);

            ambientOcclusion += isOccluded * isBiased * radiusFactor * poximityFactor * overflowFactor;
        }

        ambientOcclusion = 1.0 - clamp(ambientOcclusion / SSAOKernelSize, 0, 1);

        ambientOcclusion = HemitCurve01(ambientOcclusion, strength);

        ambientOcclusion = clamp(ambientOcclusion, 0, 1);
    }
    
    FragAO = ambientOcclusion;
} 

#endif