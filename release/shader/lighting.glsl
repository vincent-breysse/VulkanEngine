
// COMMON
// ==============================================================================

#version 450
#extension GL_ARB_separate_shader_objects : enable

const float PI = 3.14159265359f;
const int MaxPointLightCount = 256;
const int MaxPointLightCastingShadows = 64;
const int MaxLightPerTile = 64;
const float SunShadowMapSize = 50;
const float SunShadowOffset = 1000;

const float ShadowMinSampleRadius = 0;
const float ShadowMaxSampleRadius = 0.01;
const float ShadowMaxViewDist = 5.0;
const float ShadowBias = 0.2;
const vec3 ShadowSampleGrid[] = vec3[]
(
    vec3(-1, -1, -1), vec3(-1, 0, -1), vec3(-1, 1, -1),
    vec3(0, -1, -1), vec3(0, 0, -1), vec3(0, 1, -1),
    vec3(1, -1, -1), vec3(1, 0, -1), vec3(1, 1, -1),

    vec3(-1, -1, 0), vec3(-1, 0, 0), vec3(-1, 1, 0),
    vec3(0, -1, 0), vec3(0, 0, 0), vec3(0, 1, 0),
    vec3(1, -1, 0), vec3(1, 0, 0), vec3(1, 1, 0),

    vec3(-1, -1, 1), vec3(-1, 0, 1), vec3(-1, 1, 1),
    vec3(0, -1, 1), vec3(0, 0, 1), vec3(0, 1, 1),
    vec3(1, -1, 1), vec3(1, 0, 1), vec3(1, 1, 1)
);

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

vec3 AmbientColor = pow(AmbientAndIntensity.rgb, vec3(2.2f));
float AmbientIntensity = AmbientAndIntensity.w;
vec3 AmbientLuminousPower = AmbientColor * AmbientIntensity;

vec3 DirLightDirection = DirLightDirection4.xyz;
vec3 DirLightColor = pow(DirLightColorAndIntensity.rgb, vec3(2.2f));
float DirLightIntensity = DirLightColorAndIntensity.w;

vec3 ColorFilter = ColorFilterAndIntensity.rgb;
float ColorFilterIntensity = ColorFilterAndIntensity.w;

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

vec3 ViewerPos = ViewerPos4.xyz;

layout(set = 1, binding = 1) buffer readonly EntityIDBuffer
{
    uint EntityIDs[];
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
layout (set = 3, binding = 1) uniform sampler2D Image2D;
layout (set = 3, binding = 2) uniform sampler2D NormalBuffer;
layout (set = 3, binding = 3) uniform sampler2D EmissiveBuffer;
layout (set = 3, binding = 4) uniform sampler2D MaskMap;
layout (set = 3, binding = 5) uniform usampler2D MaterialHeapIndexMap;
layout (set = 3, binding = 6) uniform sampler2D SSAOMap;

layout (set = 3, binding = 7) uniform samplerCubeArray ShadowCubeArray;

layout (set = 3, binding = 8) uniform ShadowUniforms
{
    mat4 ShadowWorldClips[6 * MaxPointLightCastingShadows];
    vec4 ShadowParams[MaxPointLightCastingShadows];
};

layout (set = 3, binding = 9) uniform sampler2D BloomBuffer;
layout (set = 3, binding = 10) uniform usampler2D TileCullingTexture;
layout(set = 3, binding = 11) buffer readonly TileCullingBuffer
{
    uint PointLightIndices[];
};
layout (set = 3, binding = 12) uniform sampler2D SunShadowMap;

layout (location = 0) in vec2 VertexUV;

layout (location = 0) out vec4 FragColor;

vec3 Clamp01(vec3 v)
{
    return clamp(v, vec3(0), vec3(1));
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

float HemitCurve01(float t, float curve)
{
    return HermitMix(0, 0, 1, curve, t);
}

float InvHemitCurve01(float t, float curve)
{
    return HermitMix(0, curve, 1, 0, t);
}

vec3 GetPointLightPos(uint i)
{
    return PointLightPosAndRanges[i].xyz;
}

float GetPointLightRange(uint i)
{
    return PointLightPosAndRanges[i].w;
}

vec3 GetPointLightColor(uint i)
{
    return PointLightColorAndIntensities[i].rgb;
}

float GetPointLightPower(uint i)
{
    return PointLightColorAndIntensities[i].w;
}

vec3 GetPointLightLuminousPower(uint i)
{
    return GetPointLightColor(i) * GetPointLightPower(i);
}

float GetShadowFar(uint i)
{
    return ShadowParams[i].y;
}

vec3 ComputeFragPosition(vec2 uv)
{
    vec2 xyNS = uv * 2 - vec2(1, 1);
    float zNS = texture(DepthNSBuffer, uv).x;
    vec3 posNS = vec3(xyNS, zNS);

    vec4 unproj4 = vec4(posNS, 1) * ClipWorld;
    vec3 posWS = unproj4.xyz / unproj4.w;

    return posWS;
}

float GetPointLightFrontZ(uint lightIndex, vec3 dir)
{
    float z01 = texture(ShadowCubeArray, vec4(dir, lightIndex)).r;
    return z01 * GetShadowFar(lightIndex);
}

float GetSamplePointLightShadowWeight(uint lightIndex, vec3 fragPos, vec3 lightPos, float fragLightDist)
{
    if (CastShadows == 0)
        return 1.0;
        
    // Do not need to normalize since we're fetching a cubemap
    vec3 lightDirection = normalize(fragPos - lightPos);

    float zFromLight = fragLightDist;
    float zFront = GetPointLightFrontZ(lightIndex, lightDirection);

    return float(zFromLight < zFront + ShadowBias);
}

float ComputePointLightShadowFactor(uint lightIndex, vec3 fragPos, vec3 lightPos, vec3 lightDir, float fragLightDist)
{
    float viewDistance = length(ViewerPos - fragPos);

    float viewDistFactor = clamp(viewDistance / ShadowMaxViewDist, 0, 1);
    float sampleRadius = mix(ShadowMinSampleRadius, ShadowMaxSampleRadius, viewDistFactor);
    int nbSamples = ShadowSampleGrid.length();
    
    float shadowWeights = 0;

    for (int i = 0; i < nbSamples; ++i)
    {
        vec3 offset = ShadowSampleGrid[i] * sampleRadius;

        shadowWeights += GetSamplePointLightShadowWeight(lightIndex, fragPos + offset, lightPos, fragLightDist);
    }

    return shadowWeights / nbSamples;
}

// F0 : Base reflectivity of the material
// cosHV : cos(halfway, posToViewer), assume 0 <= cosHV <= 1 
vec3 FresnelSchlick(vec3 f0, float cosHV)
{
    return f0 + (1 - f0) * pow(1.0f - cosHV, 5);
}

// K : (roughness + 1)^2 / 8
// cosNH : cos(normal, vector), assume 0 <= cosNH <= 1 
float GeometrySchlickGGX(float cosNV, float k)
{
    return cosNV / (cosNV * (1 - k) + k);
}

// cosNV : cos(normal, posToLight), assume 0 <= cosNL <= 1 
// cosNL : cos(normal, posToViewer), assume 0 <= cosVL <= 1 
float GeometrySmith(float cosNV, float cosNL, float roughness)
{
    float k = (roughness + 1);
    k *= k / 8.f;

    return GeometrySchlickGGX(cosNV, k) * GeometrySchlickGGX(cosNL, k);
}

// cosNH : cos(normal, halfway), assume 0 <= cosNH <= 1 
float NormalDistributionGGX(float cosNH, float roughness)
{
    float r2 = roughness * roughness;
    float cosNH2 = cosNH * cosNH;
    float d = cosNH2 * (r2 - 1) + 1;
    float d2 = d * d;

    return r2 / (PI * d2);
}

vec3 ComputeBRDF(vec3 posToLight, vec3 posToViewer, vec3 normal, float cosNL, 
    vec3 surfaceColor, float surfaceRoughness, float surfaceMetallic)
{
    vec3 halfway = normalize(posToViewer + posToLight);
    float cosNH = max(dot(normal, halfway), 0);
    float cosNV = max(dot(normal, posToViewer), 0);
    float cosHV = max(dot(halfway, posToViewer), 0);

    vec3 F0 = mix(vec3(0.04f), surfaceColor, surfaceMetallic); // Standard value for dielectrics

    vec3 F = FresnelSchlick(F0, cosHV);
    float G = GeometrySmith(cosNV, cosNL, surfaceRoughness);
    float NDF = NormalDistributionGGX(cosNH, surfaceRoughness);

    vec3 DFG = NDF * F * G;
    vec3 cookTorrance = DFG / max(4 * cosNV * cosNL, 0.001f);
    vec3 specular = cookTorrance; // Ks is equal to F and is already baked in cook-torrance

    vec3 lambert = surfaceColor / PI;
    vec3 Kd = (vec3(1) - F) * (1 - surfaceMetallic); // F == Ks

    return Kd * lambert + specular;
}

float ComputeDistanceFactor(float lightDistance, float lightRange)
{
    float d = lightDistance;
    float d2 = max(d * d, 1e-5f);

    float f = min(lightDistance / max(lightRange, 0.001f), 1.f);
    float f2 = 1.0 - HemitCurve01(f, 1.0);

    return f2 / d2;
}

vec3 ComputePointLightRadiance(uint lightID, vec3 fragPos, vec3 posToViewer, vec3 normal, 
    vec3 surfaceColor, float surfaceRoughness, float surfaceMetallic)
{
    vec3 lightPos = GetPointLightPos(lightID);

    vec3 posToLight = lightPos - fragPos;
    float lightDistance = length(posToLight);
    posToLight /= lightDistance;

    float cosNL = max(dot(normal, posToLight), 0);

    vec3 brdf = ComputeBRDF(posToLight, posToViewer, normal, cosNL, 
        surfaceColor, surfaceRoughness, surfaceMetallic);

    vec3 luminousPower = GetPointLightLuminousPower(lightID);
    float distanceFactor = ComputeDistanceFactor(lightDistance, GetPointLightRange(lightID));
    float shadowFactor = (lightID < MaxPointLightCastingShadows && CastShadows != 0) ? ComputePointLightShadowFactor(lightID, fragPos, lightPos, -posToLight, lightDistance) : 1.0f;

    return luminousPower * brdf * (cosNL * distanceFactor * shadowFactor);
}

vec3 ComputePointLightIrradiance(vec3 fragPos, vec3 posToViewer, vec3 normal, 
    vec3 surfaceColor, float surfaceRoughness, float surfaceMetallic, uint tileBufferOffset, uint lightCount)
{
    vec3 radianceSum = vec3(0);

    for (uint i = 0; i < lightCount; ++i)
    {
        uint lightID = PointLightIndices[tileBufferOffset + i];
        radianceSum += ComputePointLightRadiance(lightID, fragPos, posToViewer, normal, 
            surfaceColor, surfaceRoughness, surfaceMetallic);
    }

    return radianceSum;
}

float GetSunLightFrontZ(vec2 uv)
{
    float z01 = texture(SunShadowMap, uv).r;
    return z01 * 2 * SunShadowOffset;
}

float GetSampleSunLightShadowWeight(vec3 fragPos)
{
    if (CastShadows == 0)
        return 1.0;

    vec4 posLS4 = vec4(fragPos, 1) * SunShadowWorldClip;
    if (posLS4.x < -1.0f || posLS4.x > 1.0f || posLS4.y < -1.0f || posLS4.y > 1.0f)
        return 1.0;

    float zFromLight01 = posLS4.z;
    float zFromLight = zFromLight01 * 2 * SunShadowOffset;
    vec2 shadowUV = (posLS4.xy + vec2(1, 1)) * 0.5f;
    float zFront = GetSunLightFrontZ(shadowUV);

    return float(zFromLight < zFront + ShadowBias);
}

float ComputeSunLightShadowFactor(vec3 fragPos)
{
    float viewDistance = length(ViewerPos - fragPos);
    float viewDistFactor = clamp(viewDistance / ShadowMaxViewDist, 0, 1);
    float sampleRadius = mix(ShadowMinSampleRadius, ShadowMaxSampleRadius, viewDistFactor);
    int nbSamples = ShadowSampleGrid.length();
    
    float shadowWeights = 0;

    for (int i = 0; i < nbSamples; ++i)
    {
        vec3 offset = ShadowSampleGrid[i] * sampleRadius;

        shadowWeights += GetSampleSunLightShadowWeight(fragPos + offset);
    }

    return shadowWeights / nbSamples;
}

vec3 ComputeSunLightRadiance(vec3 fragPos, vec3 posToViewer, vec3 normal, 
    vec3 surfaceColor, float surfaceRoughness, float surfaceMetallic)
{
    vec3 posToLight = -DirLightDirection;

    float cosNL = max(dot(normal, posToLight), 0);

    vec3 brdf = ComputeBRDF(posToLight, posToViewer, normal, cosNL, 
        surfaceColor, surfaceRoughness, surfaceMetallic);

    float shadowFactor = 1.0f;
    if (CastShadows != 0)
        shadowFactor = ComputeSunLightShadowFactor(fragPos);

    vec3 lightColor = DirLightColor * DirLightIntensity;

    return lightColor * brdf * cosNL * shadowFactor;
}

float GetBlurredSSAO()
{
    const float BlurStrength = 0.001f;
    const vec2 Samples[] = vec2[]
    (
        vec2(-1,1),  vec2(0,1),  vec2(1,1),
        vec2(-1,0),  vec2(0,0),  vec2(1,0),
        vec2(-1,-1), vec2(0,-1), vec2(1,-1)
    );

    int sampleCount = Samples.length();

    float ssao = 0.0f;
    for (int i = 0; i < sampleCount; ++i)
    {
        ssao += texture(SSAOMap, VertexUV + Samples[i] * BlurStrength).r;
    }

    ssao /= sampleCount;

    return ssao;
}

vec3 ApplyToneMapping(vec3 irradiance)
{
    return irradiance / pow(2.0f, Exposure);
}

vec3 ApplyColorFilter(vec3 color)
{
    return color * ColorFilter * ColorFilterIntensity;
}

vec3 ApplySaturation(vec3 color)
{
    vec3 c = vec3(0.2126, 0.7152, 0.0722);
    vec3 grayScale = vec3(dot(color, c));
    float s = max(1.f + Saturation, 0);

    return mix(grayScale, color, s);
}

vec3 ApplyContrast(vec3 color)
{
    return vec3(1 + Contrast) * (color - vec3(0.5f)) + vec3(0.5f);
}

float ComputeSunLightHardShadowFactor(vec3 fragPos)
{
    return GetSampleSunLightShadowWeight(fragPos);
}

float HenyeyGreesteinPhase(vec3 sightDirection, vec3 lightDirection, float g)
{
    float g2 = g * g;

    float cosA = dot(-lightDirection, -sightDirection);

    float x = pow(1 + g2 - 2 * g * cosA, 1.5f);

    return (1 - g2) / (4 * PI * x);
}

vec4 ComputeVolumetrics(vec3 fragPos, vec3 viewerToFrag, float viewerToFragLength, vec3 ambientColor)
{
    vec4 result = vec4(0);

    float t0 = 0;
    float t1 = viewerToFragLength;

    float maxDist = t1 - t0;
    vec3 start = ViewerPos + viewerToFrag * t0;

    const int MaxIteration = 256;
    const float FixedStep = 0.1;
    const float VolumeDensity = 0.02;
    const float ScatteringFactor = 5.0;
    const float ExtinctionFactor = 1.5;
    const float LightExtinctionFactor = 0.1;
    const float AmbientScaterringDirection = -0.2;
    const float LightScaterringDirection = -0.3;

    int iterationCount = int(min(maxDist / FixedStep, float(MaxIteration)));
    vec3 ambientScattering = ambientColor * HenyeyGreesteinPhase(viewerToFrag, -viewerToFrag, AmbientScaterringDirection);

    int it = 0;
    vec3 pos = start;

    vec3 scattering = vec3(0,0,0);
    float extinction = 1.0;

    for (int i = 0; i < iterationCount; ++i)
    {
        vec3 lightColor = DirLightColor * DirLightIntensity;
        float density = VolumeDensity;
        vec3 scatteringSum = ambientScattering;
        float lightExtinctionCoeff = LightExtinctionFactor * density;
        vec3 posToLight = -DirLightDirection;
        float cosNL = 1.0;
        vec3 brdf = vec3(1,1,1) / PI;
        float distanceToLight = 50.0; // TODO: Compute this for real using some density volume data

        float shadowFactor = ComputeSunLightHardShadowFactor(pos);
        float lightExtinction = exp(-lightExtinctionCoeff * distanceToLight);
        vec3 lightRadiance = lightColor * brdf * cosNL * shadowFactor;
        float lightPhase = HenyeyGreesteinPhase(viewerToFrag, -posToLight, LightScaterringDirection);
        vec3 lightScattering = lightPhase * lightRadiance;

        scatteringSum += lightScattering * lightExtinction;

        float scatteringCoeff = density * ScatteringFactor;
        float extinctionCoeff = density * ExtinctionFactor;

        extinction *= exp(-extinctionCoeff * FixedStep);
        scattering += extinction * scatteringCoeff * FixedStep * scatteringSum;

        pos += viewerToFrag * FixedStep;
    }

    return vec4(scattering, extinction);
}

void main () 
{ 
    vec3 fragPos = ComputeFragPosition(VertexUV);
    vec3 surfaceColor = pow(texture(Image2D, VertexUV).rgb, vec3(2.2));
    vec3 surfaceNormal = texture(NormalBuffer, VertexUV).xyz;
    vec4 surfaceMask = texture(MaskMap, VertexUV);
    vec3 surfaceEmissiveGamma = texture(EmissiveBuffer, VertexUV).rgb;
    vec3 bloomGamma = texture(BloomBuffer, VertexUV).rgb;
    uint materialHeapIndex = texture(MaterialHeapIndexMap, VertexUV).r;
    LitMaterialUniforms_t matUniforms = LitMaterialUniforms[materialHeapIndex];
    bool isUnlit = matUniforms.isUnlit == 1;

    vec2 tileCullingRatio = vec2(TileCullingResolution) / vec2(RenderTargetResolution);
    uvec2 tileCoord = uvec2(floor(gl_FragCoord.xy * tileCullingRatio));
    uint pointLightCount = texelFetch(TileCullingTexture, ivec2(tileCoord), 0).x;

    uint flatTileIndex = tileCoord.y * TileCullingResolution.x + tileCoord.x;
    uint tileBufferOffset = flatTileIndex * MaxLightPerTile;

    float surfaceMetallic = clamp(surfaceMask.x, 0.001f, 0.999f);
    float surfaceAO = surfaceMask.y;
    float surfaceDetails = surfaceMask.z;
    float surfaceRoughness = clamp(1.0 - surfaceMask.w, 0.001f, 0.999f);

    float ssao = GetBlurredSSAO();
    float ambientOcclusion = min(ssao, surfaceAO);

    vec3 viewerToFrag = fragPos - ViewerPos;
    float viewerToFragLength = length(viewerToFrag);
    viewerToFrag /= viewerToFragLength;

    vec3 ambient = AmbientLuminousPower * surfaceColor;

    vec3 irradiance = vec3(0,0,0);

    if (isUnlit)
    {
        irradiance = surfaceColor;
    }
    else
    {
        irradiance += ambient;
        irradiance += ComputeSunLightRadiance(fragPos, -viewerToFrag, surfaceNormal, surfaceColor, surfaceRoughness, surfaceMetallic);
        irradiance += ComputePointLightIrradiance(fragPos, -viewerToFrag, surfaceNormal, surfaceColor, surfaceRoughness, surfaceMetallic, tileBufferOffset, pointLightCount);
    }

    vec4 volumetrics = ComputeVolumetrics(fragPos, viewerToFrag, viewerToFragLength, ambient);
    vec3 lightScattering = volumetrics.rgb;
    float lightExtinction = volumetrics.a;

    vec3 lightScatteringLDR = pow(ApplyToneMapping(lightScattering), vec3(1 / 2.2f));

    vec3 ldr = ApplyToneMapping(irradiance);
    ldr = pow(ldr, vec3(1 / 2.2f));

    if (UseAO != 0)
    {
        ldr *= ambientOcclusion;
    }

    if (UseEmissive != 0)
    {
        ldr += surfaceEmissiveGamma;
    }

    if (UseBloom != 0)
    {
        ldr += bloomGamma * BloomColorAndIntensity.rgb * BloomColorAndIntensity.w;
    }    

    ldr = ldr * lightExtinction + lightScatteringLDR;

    ldr = Clamp01(ldr);    

     ldr = ApplyColorFilter(ldr);
     ldr = ApplySaturation(ldr);
    ldr = ApplyContrast(ldr);
    
    ldr = Clamp01(ldr);

    vec3 fragColor3 = vec3(0);

    if (DebugBuffer == 1)
    {
        fragColor3 = surfaceColor;
    }
    else if (DebugBuffer == 2)
    {
        fragColor3 = surfaceNormal;
    }
    else if (DebugBuffer == 3)
    {
        fragColor3 = vec3(1,1,1) * surfaceMetallic;
    }
    else if (DebugBuffer == 4)
    {
        fragColor3 = vec3(1,1,1) * surfaceRoughness;
    }
    else if (DebugBuffer == 5)
    {
        fragColor3 = vec3(1,1,1) * surfaceAO;
    }
    else if (DebugBuffer == 6)
    {
        fragColor3 = vec3(1,1,1) * ambientOcclusion;
    }
    else if (DebugBuffer == 7)
    {
        fragColor3 = (pointLightCount / float(MaxLightPerTile)) * vec3(1,1,1);
    }
    else
    {
        fragColor3 = ldr;
    }

    FragColor = vec4(fragColor3, 1);
}

#endif