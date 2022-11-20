
// COMMON
// ==============================================================================

#version 450
#extension GL_ARB_separate_shader_objects : enable

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

void main()
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

layout (set = 3, binding = 0) uniform sampler2D Image2D;

layout (location = 0) in vec2 VertexUV;

layout (location = 0) out vec4 FragColor;

void main() 
{ 
    FragColor = texture(Image2D, VertexUV);
}

#endif