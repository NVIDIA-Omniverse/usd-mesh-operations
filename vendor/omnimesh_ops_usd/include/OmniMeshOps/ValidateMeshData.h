// SPDX-FileCopyrightText: Copyright (c) 2023-2025 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
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

#include <string>

namespace omo
{

template <Residency R>
class MeshData;

/// @brief Check if the given MeshData is manifold
/// @param mesh_data The MeshData to check
/// @return ValidationResult will contain true if MeshData is manifold, false otherwise
template <Residency R>
OMO_EXPORT
ValidationResult
checkManifold(
    const MeshData<R>& mesh_data);

/// @brief Check if the given MeshData has no mergeable vertices
/// @note The merging algorithm may merge vertices up to a constant
///     approximation factor (4 by default) times the given tolerance apart.
///     No approximation is used when tolerance is 0.
/// @param mesh_data The MeshData to check
/// @param tolerance The merge tolerance (vertices are considered mergeable if closer than the tolerance)
/// @param merge_neighbors Merge coincident vertices that are neighbors around some face
/// @param merge_boundaries Merge coincident *boundary* vertices. Here a
///     boundary vertex is one that is incident to an *unshared* edge on
///     one of the faces referencing it.
/// @return ValidationResult will contain true if MeshData has no mergeable vertices, false otherwise
template <Residency R>
OMO_EXPORT
ValidationResult
checkNoMergeableCoincidentVertices(
    const MeshData<R>& mesh_data,
    double tolerance = 0.0,
    meshdata::MergeNeighbors merge_neighbors = { true },
    meshdata::MergeBoundaries merge_boundaries = { true });

/// @brief Check if the given MeshData has any degenerate edges
/// @param mesh_data The MeshData to check
/// @return ValidationResult will contain true if MeshData has no degenerate edges, false otherwise
template <Residency R>
OMO_EXPORT
ValidationResult
checkNoDegenerateEdges(const MeshData<R>& mesh_data);

/// @brief Check if the given MeshData has any degenerate faces
/// @param mesh_data The MeshData to check
/// @return ValidationResult will contain true if MeshData has no degenerate faces, false otherwise
template <Residency R>
OMO_EXPORT
ValidationResult
checkNoDegenerateFaces(const MeshData<R>& mesh_data);

/// @brief Check if the given MeshData has any isolated vertices
/// @param mesh_data The MeshData to check
/// @return ValidationResult will contain true if MeshData has no isolated vertices, false otherwise
template <Residency R>
OMO_EXPORT
ValidationResult
checkNoIsolatedVertices(const MeshData<R>& mesh_data);

/// @brief Check if the given MeshData has any duplicate faces
/// @param mesh_data The MeshData to check
/// @return ValidationResult will contain true if MeshData has no duplicate faces, false otherwise
template <Residency R>
OMO_EXPORT
ValidationResult
checkNoDuplicateFaces(const MeshData<R>& mesh_data);

} // namespace omo
