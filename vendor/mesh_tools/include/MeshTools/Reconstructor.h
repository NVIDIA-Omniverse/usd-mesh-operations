// SPDX-FileCopyrightText: Copyright (c) 2024-2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
// SPDX-License-Identifier: LicenseRef-NvidiaProprietary
//
// NVIDIA CORPORATION, its affiliates and licensors retain all intellectual
// property and proprietary rights in and to this material, related
// documentation and any modifications thereto. Any use, reproduction,
// disclosure or distribution of this material and related documentation
// without an express license agreement from NVIDIA CORPORATION or
// its affiliates is strictly prohibited.
#pragma once

#include "Defines.h"
#include "Mesh.h"
#include "Vec.h"

#include <memory>
#include <string>
#include <vector>


namespace MeshTools
{


struct ReconstructorMeshDescriptor
{
    /** @brief The transform applied to the input points.
     */
    Mat44 transform = Mat44(Identity);

    /** @brief Vertex positions (point cloud) or mesh vertices when using polygon input.
     */
    const Vec3* points = nullptr;

    /** @brief Optional per-point RGBA colors in the same order as @ref points.
     * When non-null, must have @ref numPoints entries. When null, the reconstructor
     * has no per-point color input for this mesh.
     */
    const Vec4* colors = nullptr;

    /** @brief The number of points / vertices in the mesh */
    size_t numPoints = 0;

    /** @brief The indices of the faces of the mesh.
     */
    const int* faceIndices = nullptr;

    /** @brief Number of entries in @ref faceIndices */
    int numIndices = 0;

    /** @brief Number of vertices per face;
     */
    const int* faceSizes = nullptr;

    /** @brief Number of faces (length of @ref faceSizes). */
    int numFaces = 0;
};

enum class ReconstructorMethod
{
    Voxel,
    Delaunay,
};

struct ReconstructorParameters
{
    /** @var type
     * The type of reconstructor to use.
     */
    ReconstructorMethod method = ReconstructorMethod::Voxel;

    /** @var useGpu
     * If true, the reconstruction will be performed on the GPU.
     */
    bool useGpu = true;

    /** @var gpuId
     * The ID of the GPU to use for reconstruction.
     */
    int gpuId = 0;

    /** @var spacing
     * The resolution of the voxel grid used for the reconstruction.
     */
    float spacing = 0.01f;

    /** @var maximumPointsPerTile
     * For very large point clouds, the reconstruction can be split into tiles to avoid memory issues.
     * If the value is zero, no tiling is performed.
     */
    int maximumPointsPerTile = 10000000; // 10M

    /** @var removeInnerSurfaces
     * This parameter is only available if the reconstruction method is Voxel based.
     * Removes all surfaces that are inside closed surfaces.
     */
    bool removeInnerSurfaces = true;

    /** @var shrinkMesh
     * If true, mesh vertices are shrunk along their normals toward the input geometry at each optimization iteration.
     */
    bool shrinkMesh = true;

    /** @var relativeSurfaceThickness
     * This parameter defines the thickness of the surface.
     * For Voxel based reconstruction, the mesh is the surface of the voxel grid
     * which is two-sided.
     * The value is relative to the spacing parameter and must be between 0.1 and 1.0
     * This is only available if the reconstruction method is Voxel based.
     */
    float relativeSurfaceThickness = 0.1f;

    /** @var projectMeshAlongVertexNormals
     * When projecting mesh vertices to the closest point/triangle, move the vertex along its (current) mesh normal.
     * This is intended for shrink-wrap behavior and can help entering concavities compared to pure closest-point
     * offsetting.
     *
     * Only used for voxel-based reconstruction.
     */
    bool projectMeshAlongVertexNormals = true;

    /** @var targetEdgeLength
     * The target edge length of the mesh.
     * The value is relative to the spacing parameter.
     * This is only available if the reconstruction method is Voxel based.
     */
    float relativeTargetEdgeLength = 0.8f;

    /** @var numContractionIterations
     * The number of contraction iterations to perform before the first projection.
     * This is only available if the reconstruction method is Voxel based.
     */
    int numContractionIterations = 3;

    /** @var numOptimizationIterations
     * The number of optimization iterations to perform.
     * Optimization is performed after the reconstruction to improve the quality of the mesh.
     */
    int numOptimizationIterations = 1;

    /** @var optimizeEdges
     * If true, short edges are collapsed, edges are flipped, and long edges are split during each
     * optimization iteration.
     */
    bool optimizeEdges = true;

    /** @var smoothMesh
     * If true, tangential smoothing is applied during each optimization iteration.
     */
    bool smoothMesh = true;

    /** @var featureEdgeThreshold
     * The threshold for feature edges. Values are in the range [0, 1].
     * These are preserved during edge collapse in optimization.
     * The value corresponds to the dot product of the  normals of adjacent triangles.
     * A value of 0.0 means no feature edges are preserved.
     */
    float featureEdgeThreshold = 0.95f;

    /** @var returnConnections
     * If true, the reconstructor will return the point-point connections.
     */
    bool returnConnections = false;

    /** @var returnPointNormalSegments
     * If true, the reconstructor will return the normals of the points.
     * They are only available if the reconstruction method is Delaunay based.
     */
    bool returnPointNormalSegments = false;

    /** @var removeNonmanifoldGeometry
     * If true, the reconstructor will remove non-manifold geometry.
     * This is only available if the reconstruction method is Delaunay based.
     */
    bool removeNonmanifoldGeometry = true;

    /** @var fillHoles
     * If true, the reconstructor will fill holes in the mesh.
     * This is only available if the reconstruction method is Delaunay based.
     */
    bool fillHoles = true;
};


enum class ReconstructorDebugLineType
{
    None,
    Normal,
    NeighborConnection,
    NumLineTypes
};


/**
 * @brief Reconstructs a triangle mesh from a point cloud
 *
 */
class Reconstructor
{
public:
    /**
     * @brief Creates a new reconstructor.
     */
    MESHTOOLS_EXPORT Reconstructor();
    MESHTOOLS_EXPORT ~Reconstructor();

    /**
     * @brief Reconstructs a triangle mesh from a point cloud.
     * @param params The parameters for the reconstruction.
     * @param meshes The meshes to reconstruct.
     * @param numMeshes The number of meshes to reconstruct.
     * @return True if the reconstruction was successful, false otherwise.
     */
    MESHTOOLS_EXPORT bool reconstruct(const ReconstructorParameters& params,
                                      const ReconstructorMeshDescriptor* meshes,
                                      int numMeshes);
    /**
     * @brief Returns the vertices of the mesh.
     * @return The vertices of the mesh.
     */
    MESHTOOLS_EXPORT const Vec3* getVertices() const;

    /**
     * @brief Returns the number of vertices of the mesh.
     * @return The number of vertices of the mesh.
     */
    MESHTOOLS_EXPORT size_t getNumVertices() const;

    /**
     * @brief Returns per-vertex normals of the reconstructed triangle mesh.
     */
    MESHTOOLS_EXPORT const Vec3* getNormals() const;

    /**
     * @brief Number of normals returned by getNormals.
     */
    MESHTOOLS_EXPORT size_t getNumNormals() const;

    /**
     * @brief Per-vertex RGBA for the reconstructed mesh (same count and order as getVertices).
     * Defaults to opaque white when no input colors or voxel path without projection to the point cloud.
     */
    MESHTOOLS_EXPORT const Vec4* getVertexColors() const;

    MESHTOOLS_EXPORT size_t getNumVertexColors() const;

    /**
     * @brief Returns the triangle indices of the mesh.
     * @return The triangle indices of the mesh.
     */
    MESHTOOLS_EXPORT const int* getTriangleIndices() const;

    /**
     * @brief Returns the number of triangles of the mesh.
     * @return The number of triangles of the mesh.
     */
    MESHTOOLS_EXPORT size_t getNumTriangles() const;

    /**
     * @brief Returns the number of meshes.
     * @return The number of meshes.
     */
    MESHTOOLS_EXPORT size_t getNumMeshes() const;

    /**
     * @brief Returns point-point neighbor pairs from the Delaunay neighbor graph.
     * Each pair contains two global input point indices (see getInputPoints).
     * Only available for Delaunay reconstruction with returnConnections enabled.
     */
    MESHTOOLS_EXPORT const int* getConnections() const;

    /**
     * @brief Returns the number of connections.
     * @return The number of connections.
     */
    MESHTOOLS_EXPORT size_t getNumConnections() const;

    /**
     * @brief Global input point positions used for reconstruction (mesh transforms applied,
     * all meshes concatenated in reconstruct() order). Connection indices refer to this array.
     */
    MESHTOOLS_EXPORT const Vec3* getInputPoints() const;

    MESHTOOLS_EXPORT size_t getNumInputPoints() const;

    /**
     * @brief Number of reconstruction tiles from the last reconstruct() call.
     * Returns 1 when tiling is disabled.
     */
    MESHTOOLS_EXPORT size_t getNumTiles() const;

    /**
     * @brief Axis-aligned clip bounds used for each reconstruction tile.
     * Length matches getNumTiles(), in the same order tiles were processed.
     * These are the bounds passed to point collection (including halo overlap when tiling is enabled,
     * clamped to the global scene bounds). When tiling is disabled, returns a single entry for the
     * region used for the whole reconstruction.
     */
    MESHTOOLS_EXPORT const Bounds3* getTiles() const;

    /**
     * @brief Returns segments to render point normals. This is only available if the reconstruction method is Delaunay
     * based. Also returnPointNormalSegments must be true in the reconstruction parameters.
     * @return An array of Vec3 pairs ready to draw using GL_LINES.
     */
    MESHTOOLS_EXPORT const Vec3* getPointNormalSegments() const;

    /**
     * @brief Number of Vec3 endpoints in getPointNormalSegments (two per line segment, suitable for GL_LINES).
     */
    MESHTOOLS_EXPORT size_t getNumPointNormalSegments() const;

    /**
     * @brief Raycasts the mesh.
     * @param ray The ray to raycast.
     * @param distance The distance to the hit point.
     * @param triNr The index of the hit triangle, or -1 if no hit.
     * @param vertexNr The vertex index of the hit triangle closest to the hit point, or -1 if no hit.
     * @return True if a hit was found, false otherwise.
     */
    MESHTOOLS_EXPORT bool raycast(const Ray& ray, float& distance, int& triNr, int& vertexNr) const;

    /*! \cond PRIVATE */
protected:
    class ReconstructorImpl;
    MESHTOOLS_EXPORT Reconstructor(const ReconstructorImpl&);
    std::unique_ptr<ReconstructorImpl> m_impl;
    /*! \endcond */
};

}
