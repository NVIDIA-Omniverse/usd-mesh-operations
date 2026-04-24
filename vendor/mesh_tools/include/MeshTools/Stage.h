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

#include "Curve.h"
#include "Defines.h"
#include "Mesh.h"
#include "Vec.h"

#include <memory>
#include <string>
#include <vector>

namespace MeshTools
{


/**
 * @brief Represents a set of meshes.
 *
 */
class Stage
{
public:
    /// Constructs an empty Stage
    MESHTOOLS_EXPORT Stage();
    MESHTOOLS_EXPORT ~Stage();

    /**
     * @brief Clears the stage.
     */
    MESHTOOLS_EXPORT void clear();

    /**
     * @brief Returns true if the stage is empty.
     */
    MESHTOOLS_EXPORT bool empty() const;

    /**
     * @brief Initializes the stage with a set of meshes, optional prototypes, and optional curves.
     */
    MESHTOOLS_EXPORT void init(std::vector<std::shared_ptr<Mesh>> meshes,
                               const std::vector<std::shared_ptr<Mesh>>* prototypes = nullptr,
                               const std::vector<std::shared_ptr<Curve>>* curves = nullptr);

    /**
     * @brief Returns all the meshes of the stage.
     */
    MESHTOOLS_EXPORT const std::vector<std::shared_ptr<Mesh>>& meshes() const;

    /**
     * @brief Returns all the prototype meshes of the stage.
     */
    MESHTOOLS_EXPORT const std::vector<std::shared_ptr<Mesh>>& prototypes() const;

    /**
     * @brief Returns the mesh at the given index.
     */
    MESHTOOLS_EXPORT std::shared_ptr<Mesh> mesh(size_t index) const;

    /**
     * @brief Returns all the curves of the stage.
     */
    MESHTOOLS_EXPORT const std::vector<std::shared_ptr<Curve>>& curves() const;

    /**
     * @brief Returns the curve at the given index.
     */
    MESHTOOLS_EXPORT std::shared_ptr<Curve> curve(size_t index) const;

    /**
     * @brief Find the closest face that is intersected by the given ray.
     * notifyMeshChange() must be called after changing the geometry or transforms of meshes.
     * @param ray The ray to intersect with the mesh
     * @param t The distance along the ray to the intersection point
     * @param meshNr The number of the mesh that was intersected
     * @param faceNr The number of the face that was intersected
     * @param clip (Optional) A clip plane to clip the mesh against
     * @return Boolean indicating if an intersection was found
     */
    MESHTOOLS_EXPORT bool raycast(
        const Ray& ray, float& t, size_t& meshNr, size_t& faceNr, Vec3 clip = Vec3(MaxFloat, MaxFloat, MaxFloat)) const;

    /**
     * @brief Returns the axis-aligned bounding box of the union of all meshes.
     * notifyMeshChange() must be called after changing the geometry or transforms of meshes.
     */
    MESHTOOLS_EXPORT const Bounds3& getAABB() const;

    /**
     * @brief Notifies the stage that some meshes have changed.
     * This will trigger a recomputation of the stage bounds and BVH on the next call to getAABB or raycast.
     */
    MESHTOOLS_EXPORT void notifyMeshChange();

    /**
     * @brief Clusters islands of all meshes by merging islands that are smaller than the given size.
     * The result is stored in the island number of each face.
     * @param clusterSize The desired size of the clusters
     */
    MESHTOOLS_EXPORT void clusterIslands(float clusterSize);

    /**
     * @brief Splits the meshes of the stage into their islands
     * @param useWeldedVertices If true, co-located vertices are considered to be the same vertex for island splitting
     * @param maxMeshSize Meshes smaller than this size are not split
     * @param minIslandSize All islands of a mesh smaller than this size are gathered into one mesh
     */
    MESHTOOLS_EXPORT bool splitMeshIslands(bool useWeldedVertices, float maxMeshSize, float minIslandSize);

    /*! \cond PRIVATE */
protected:
    class StageImpl;
    MESHTOOLS_EXPORT Stage(const StageImpl&);
    std::unique_ptr<StageImpl> m_impl;
    /*! \endcond */
};

}
