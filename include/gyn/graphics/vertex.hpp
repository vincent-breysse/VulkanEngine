#pragma once

#include <gyn/graphics/common.hpp>

namespace Gyn
{
    constexpr ea::array CubeVertices
    {
        // Front face
        Vertex{ float3(-1.0f, -1.0f, -1.0f),  float3(0, 0, -1),    float2(0.0f, 1.0f),    float3(1, 0, 0) },
        Vertex{ float3(-1.0f, 1.0f, -1.0f),   float3(0, 0, -1),    float2(0.0f, 0.0f),    float3(1, 0, 0) },
        Vertex{ float3(1.0f, 1.0f, -1.0f),    float3(0, 0, -1),    float2(1.0f, 0.0f),    float3(1, 0, 0) },
        Vertex{ float3(1.0f, -1.0f, -1.0f),   float3(0, 0, -1),    float2(1.0f, 1.0f),    float3(1, 0, 0) },

        // Back face
        Vertex{ float3(1.0f, -1.0f, 1.0f),    float3(0, 0, 1),     float2(0.0f, 1.0f),    float3(-1, 0, 0) },
        Vertex{ float3(1.0f, 1.0f, 1.0f),     float3(0, 0, 1),     float2(0.0f, 0.0f),    float3(-1, 0, 0) },
        Vertex{ float3(-1.0f, 1.0f, 1.0f),    float3(0, 0, 1),     float2(1.0f, 0.0f),    float3(-1, 0, 0) },
        Vertex{ float3(-1.0f, -1.0f, 1.0f),   float3(0, 0, 1),     float2(1.0f, 1.0f),    float3(-1, 0, 0) },

        // Left face
        Vertex{ float3(-1.0f, -1.0f, 1.0f),   float3(-1, 0, 0),    float2(0.0f, 1.0f),    float3(0, 0, -1) },
        Vertex{ float3(-1.0f, 1.0f, 1.0f),    float3(-1, 0, 0),    float2(0.0f, 0.0f),    float3(0, 0, -1) },
        Vertex{ float3(-1.0f, 1.0f, -1.0f),   float3(-1, 0, 0),    float2(1.0f, 0.0f),    float3(0, 0, -1) },
        Vertex{ float3(-1.0f, -1.0f, -1.0f),  float3(-1, 0, 0),    float2(1.0f, 1.0f),    float3(0, 0, -1) },

        // Right face
        Vertex{ float3(1.0f, -1.0f, -1.0f),   float3(1, 0, 0),     float2(0.0f, 1.0f),   float3(0, 0, 1) },
        Vertex{ float3(1.0f, 1.0f, -1.0f),    float3(1, 0, 0),     float2(0.0f, 0.0f),   float3(0, 0, 1) },
        Vertex{ float3(1.0f, 1.0f, 1.0f),     float3(1, 0, 0),     float2(1.0f, 0.0f),   float3(0, 0, 1) },
        Vertex{ float3(1.0f, -1.0f, 1.0f),    float3(1, 0, 0),     float2(1.0f, 1.0f),   float3(0, 0, 1) },

        // Bottom face
        Vertex{ float3(-1.0f, -1.0f, 1.0f),   float3(0, -1, 0),    float2(0.0f, 1.0f),   float3(1, 0, 0) },
        Vertex{ float3(-1.0f, -1.0f, -1.0f),  float3(0, -1, 0),    float2(0.0f, 0.0f),   float3(1, 0, 0) },
        Vertex{ float3(1.0f, -1.0f, -1.0f),   float3(0, -1, 0),    float2(1.0f, 0.0f),   float3(1, 0, 0) },
        Vertex{ float3(1.0f, -1.0f, 1.0f),    float3(0, -1, 0),    float2(1.0f, 1.0f),   float3(1, 0, 0) },

        // Top face
        Vertex{ float3(-1.0f, 1.0f, -1.0f),  float3(0, 1, 0),     float2(0.0f, 1.0f),   float3(-1, 0, 0) },
        Vertex{ float3(-1.0f, 1.0f, 1.0f),   float3(0, 1, 0),     float2(0.0f, 0.0f),   float3(-1, 0, 0) },
        Vertex{ float3(1.0f, 1.0f, 1.0f),    float3(0, 1, 0),     float2(1.0f, 0.0f),   float3(-1, 0, 0) },
        Vertex{ float3(1.0f, 1.0f, -1.0f),   float3(0, 1, 0),     float2(1.0f, 1.0f),   float3(-1, 0, 0) },
    };

    constexpr ea::array<uint16, 36> CubeIndices
    {
        0,1,2,
        2,3,0,

        4,5,6,
        6,7,4,

        8,9,10,
        10,11,8,

        12,13,14,
        14,15,12,

        16,17,18,
        18,19,16,

        20,21,22,
        22,23,20
    };

    constexpr ea::array TriangleVertices
    {
        Vertex{ float3(-1.0f, -1.0f, 0.0f),  float3(0, 0, -1),        float2(0.0f, 1.0f),    float3(1, 0, 0) },
        Vertex{ float3(-1.0f, 1.0f, 0.0f),   float3(0, 0, -1),        float2(0.0f, 0.0f),    float3(1, 0, 0) },
        Vertex{ float3(1.0f, 1.0f, 0.0f),    float3(0, 0, -1),        float2(1.0f, 0.0f),    float3(1, 0, 0) },
    };

    constexpr ea::array<uint16, 3> TriangleIndices
    {
        0,1,2
    };

    constexpr ea::array Triangle2Vertices
    {
        Vertex{ float3(1.0f, 1.0f, -0.0f ),    float3(0, 0, -1),        float2(1.0f, 0.0f),    float3(1, 0, 0) },
        Vertex{ float3(1.0f, -1.0f, -0.0f),    float3(0, 0, -1),        float2(1.0f, 1.0f),    float3(1, 0, 0) },
        Vertex{ float3(-1.0f, -1.0f, 0.0f),    float3(0, 0, -1),        float2(0.0f, 1.0f),    float3(1, 0, 0) },
    };

    constexpr ea::array<uint16, 3> Triangle2Indices
    {
        0,1,2
    };

    constexpr ea::array Triangle3Vertices
    {
        Vertex{ float3(-1.0f, -1.0f, 0.0f),   float3(0, 0, -1),        float2(0.0f, 0.0f),    float3(1, 0, 0) },
        Vertex{ float3(0.0f, 1.0f, 0.0f),     float3(0, 0, -1),        float2(0.5f, 1.0f),    float3(1, 0, 0) },
        Vertex{ float3(1.0f, -1.0f, 0.0f),    float3(0, 0, -1),        float2(1.0f, 0.0f),    float3(1, 0, 0) },
    };

    constexpr ea::array<uint16, 3> Triangle3Indices
    {
        0,1,2
    };

    constexpr ea::array Triangle4Vertices
    {
        Vertex{ float3(-1.0f, 1.0f, 0.0f),   float3(0, 0, -1),         float2(0.0f, 1.0f),    float3(1, 0, 0) },
        Vertex{ float3(1.0f, 1.0f, 0.0f),    float3(0, 0, -1),         float2(1.0f, 1.0f),    float3(1, 0, 0) },
        Vertex{ float3(0.0f, -1.0f, 0.0f),   float3(0, 0, -1),         float2(0.5f, 0.0f),    float3(1, 0, 0) },
    };

    constexpr ea::array<uint16, 3> Triangle4Indices
    {
        0,1,2
    };
}