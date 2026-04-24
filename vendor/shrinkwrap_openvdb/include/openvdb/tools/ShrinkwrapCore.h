// SPDX-FileCopyrightText: Copyright (c) 2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
// SPDX-License-Identifier: LicenseRef-NvidiaProprietary
//
/// @file    tools/ShrinkwrapCore.h
///
/// @brief   OpenVDB-agnostic interface for shrink-wrapping a polygon soup
///          into a watertight mesh using level set operations.
///
/// @details The public types use only standard library types so that callers
///          need not depend on OpenVDB headers directly.

#ifndef OPENVDB_TOOLS_SHRINKWRAP_CORE_HAS_BEEN_INCLUDED
#define OPENVDB_TOOLS_SHRINKWRAP_CORE_HAS_BEEN_INCLUDED

#include <array>
#include <string>
#include <vector>


namespace openvdb {
namespace shrinkwrap {


////////////////////////////////////////


/// @brief Parameters controlling the shrinkwrap operation.
struct ShrinkwrapParams {
    float voxelSize = 0.1f;          ///< Voxel size in world units (pre-computed by caller)
    float halfWidth = 3.0f;          ///< Half the width of the narrow band, in voxel units
    float erode = 8.0f;              ///< Number of erosion steps
    float threshold = 0.0f;          ///< Closing threshold for shrink-wrap
    float isovalue = 0.0f;           ///< Isosurface value for mesh extraction
    float adaptivity = 0.0f;         ///< Adaptive meshing threshold [0,1]
    bool  extractLodPyramid = false; ///< Extract meshes for all LOD levels
};


/// @brief A simple polygon soup: points, triangles, and quads.
struct ShrinkwrapMesh {
    std::vector<std::array<float, 3>> points;
    std::vector<std::array<int, 3>>   triangles;
    std::vector<std::array<int, 4>>   quads;
};


/// @brief Per-grid metadata produced by shrinkwrapMesh().
struct ShrinkwrapGridInfo {
    float  voxelSize;    ///< Voxel size used for this grid
    int    dim;          ///< Effective grid dimension
    size_t activeVoxels; ///< Number of active voxels
};


/// @brief Result of a shrinkwrapMesh() call.
struct ShrinkwrapResult {
    std::vector<ShrinkwrapMesh>     lodMeshes; ///< Output meshes (one per LOD level)
    std::vector<ShrinkwrapGridInfo> gridInfos; ///< Per-grid metadata
    std::string error;                         ///< Non-empty on failure
};


////////////////////////////////////////


/// @brief  Convert a polygon soup into a watertight shrink-wrapped mesh.
///
/// @details All OpenVDB details are hidden behind this interface so that
///          callers need not depend on OpenVDB types directly.
///
/// @param inputMesh  Input polygon soup (points, triangles, quads).
/// @param params     Parameters controlling voxel size, erosion, etc.
/// @return A ShrinkwrapResult containing the output meshes and grid metadata,
///         or a non-empty @c error string on failure.
ShrinkwrapResult shrinkwrapMesh(
    const ShrinkwrapMesh& inputMesh,
    const ShrinkwrapParams& params);


} // namespace shrinkwrap
} // namespace openvdb

#endif // OPENVDB_TOOLS_SHRINKWRAP_CORE_HAS_BEEN_INCLUDED
