#pragma once

#include <gyn/common/fwd.hpp>

namespace Gyn
{
    class Image2D;
    class ImageCube;
    class ImageCubeArray;
    class Material;
    class Mesh;
    class Model;

    struct SubMeshLoading;
    struct ModelLoading;
    struct LitMaterialUniforms;

    class GraphicsEngine;
    class MatMeshRefCountManager;
    class MatMeshRefCounter;
    struct MatMeshMetadataCache;
    struct PointLightCommon;
    struct PointLight;
    class RenderAssetManager;
    class MatMeshQuery;
    class MatMeshQueryBuilder;

    struct RenderEntityChunk;
    struct RenderContext;
    struct PointLightChunk;

    struct FrustrumPlanesSOA;
    struct PointLightCulling;
    struct CullingOutput;
    struct FrameCulling;
}