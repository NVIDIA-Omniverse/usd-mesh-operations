// SPDX-FileCopyrightText: Copyright (c) 2024-2025 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
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
    /** @brief The points of the mesh */
    const Vec3* points = nullptr;

    /** @brief The number of points in the mesh */
    int numPoints = 0;

    /** @brief The transform applied to the input points.
     */
    Mat44 transform = Mat44(Identity);

    /** @brief The radius used to search for neighboring points during reconstruction.
     * If zero, the searchRadius defined in ReconstructorParameters is used.
     */
    float searchRadius = 0.0f;
};

struct ReconstructorParameters
{
    /** @var useGpu
     * If true, the reconstruction will be performed on the GPU.
     */
    bool useGpu = true;

    /** @var gpuId
     * The ID of the GPU to use for reconstruction.
     */
    int gpuId = 0;

    /** @var searchRadius
     * The maxium radius used to search for neighboring points during reconstruction.
     */
    float maxSearchRadius = 0.1f;

    /** @var returnConnections
     * If true, the reconstructor will return the point-triangle connections.
     */
    bool returnConnections = false;
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

    MESHTOOLS_EXPORT bool reconstruct(const ReconstructorParameters& params,
                                      const ReconstructorMeshDescriptor* meshes,
                                      int numMeshes);

    MESHTOOLS_EXPORT const Vec3* getPoints(int meshNr = -1) const;

    MESHTOOLS_EXPORT const Vec3* getNormals(int meshNr = -1) const;

    MESHTOOLS_EXPORT size_t getNumPoints(int meshNr = -1) const;

    MESHTOOLS_EXPORT const int* getTriangleIndices(int meshNr = -1) const;

    MESHTOOLS_EXPORT size_t getNumTriangles(int meshNr = -1) const;

    MESHTOOLS_EXPORT size_t getNumMeshes() const;

    MESHTOOLS_EXPORT const int* getConnections() const;

    MESHTOOLS_EXPORT size_t getNumConnections() const;


    /*! \cond PRIVATE */
protected:
    class ReconstructorImpl;
    MESHTOOLS_EXPORT Reconstructor(const ReconstructorImpl&);
    std::unique_ptr<ReconstructorImpl> m_impl;
    /*! \endcond */
};

}
