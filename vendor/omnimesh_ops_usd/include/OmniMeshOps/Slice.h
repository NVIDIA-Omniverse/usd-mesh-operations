// SPDX-FileCopyrightText: Copyright (c) 2023-2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
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

#include <array>
#include <string>
#include <vector>

namespace omo
{
template <Residency R>
class MeshData;

/// @brief Slices the given mesh data
/// @param mesh_data The mesh data to slice
/// @param up_vector The up vector, to which the heights refer to
/// @param heights Heights to cut at, in mesh local coordinate space
/// @return The sliced mesh data, including all the attachments from the original mesh
template <Residency R>
OMO_EXPORT //
MeshData<R>
slice(const MeshData<R>& mesh_data, std::array<double, 3> up_vector, std::vector<double> heights);

/// @brief Slices the given mesh data
/// @param mesh_data The mesh data to slice
/// @param transferred_attachments The names of the attributes/subsets to transfer to the output mesh data
/// @param up_vector The up vector, to which the heights refer to
/// @param heights Heights to cut at, in mesh local coordinate space
/// @return The sliced mesh data
template <Residency R>
OMO_EXPORT //
MeshData<R>
slice(
    const MeshData<R>& mesh_data,
    const std::vector<std::string>& transferred_attachments,
    std::array<double, 3> up_vector,
    std::vector<double> heights);

/// @brief Slices the given mesh data into grid cells
/// @param mesh_data The mesh data to slice
/// @param world_grid_cell_lengths The lengths of the the grid cube sides in world-space (a zero will skip slicing in
/// that direction)
/// @param world_grid_origin The origin of the grid in world-space
/// @param up_x_axis Custom up axis for the operation (a zero-length up-vector will skip slicing in that direction)
/// @param up_y_axis Custom up axis for the operation (a zero-length up-vector will skip slicing in that direction)
/// @param up_z_axis Custom up axis for the operation (a zero-length up-vector will skip slicing in that direction)
/// @return The sliced mesh data
template <Residency R>
OMO_EXPORT //
MeshData<R>
gridSlice(
    const MeshData<R>& mesh_data,
    std::array<double, 3> world_grid_cell_lengths,
    std::array<double, 3> world_grid_origin = { 0, 0, 0 },
    std::array<double, 3> up_x_axis = { 1, 0, 0 },
    std::array<double, 3> up_y_axis = { 0, 1, 0 },
    std::array<double, 3> up_z_axis = { 0, 0, 1 });

/// @brief Slices the given mesh data into grid cells
/// @param mesh_data The mesh data to slice
/// @param transferred_attachments The names of the attributes/subsets to transfer to the output mesh data
/// @param world_grid_cell_lengths The lengths of the the grid cube sides in world-space (a zero will skip slicing in
/// that direction)
/// @param world_grid_origin The origin of the grid in world-space
/// @param up_x_axis Custom up axis for the operation (a zero-length up-vector will skip slicing in that direction)
/// @param up_y_axis Custom up axis for the operation (a zero-length up-vector will skip slicing in that direction)
/// @param up_z_axis Custom up axis for the operation (a zero-length up-vector will skip slicing in that direction)
/// @return The sliced mesh data
template <Residency R>
OMO_EXPORT //
MeshData<R>
gridSlice(
    const MeshData<R>& mesh_data,
    const std::vector<std::string>& transferred_attachments,
    std::array<double, 3> world_grid_cell_lengths,
    std::array<double, 3> world_grid_origin = { 0, 0, 0 },
    std::array<double, 3> up_x_axis = { 1, 0, 0 },
    std::array<double, 3> up_y_axis = { 0, 1, 0 },
    std::array<double, 3> up_z_axis = { 0, 0, 1 });

/// @brief Clips the given mesh data to the given AABB
/// @param mesh_data The mesh data to clip
/// @param world_min The minimum corner of the world-space AABB to clip to
/// @param world_max The maximum corner of the world-space AABB to clip to
/// @param up_x_axis Custom up axis for the operation
/// @param up_y_axis Custom up axis for the operation
/// @param up_z_axis Custom up axis for the operation
/// @return The clipped mesh data, including all the attachments from the original mesh
template <Residency R>
OMO_EXPORT //
MeshData<R>
boxClip(
    const MeshData<R>& mesh_data,
    std::array<double, 3> world_min,
    std::array<double, 3> world_max,
    std::array<double, 3> up_x_axis = { 1, 0, 0 },
    std::array<double, 3> up_y_axis = { 0, 1, 0 },
    std::array<double, 3> up_z_axis = { 0, 0, 1 });

/// @brief Clips the given mesh data to the given AABB
/// @param mesh_data The mesh data to clip
/// @param transferred_attachments The names of the attributes/subsets to transfer to the output mesh data
/// @param world_min The minimum corner of the world-space AABB to clip to
/// @param world_max The maximum corner of the world-space AABB to clip to
/// @param up_x_axis Custom up axis for the operation
/// @param up_y_axis Custom up axis for the operation
/// @param up_z_axis Custom up axis for the operation
/// @return The clipped mesh data
template <Residency R>
OMO_EXPORT //
MeshData<R>
boxClip(
    const MeshData<R>& mesh_data,
    const std::vector<std::string>& transferred_attachments,
    std::array<double, 3> world_min,
    std::array<double, 3> world_max,
    std::array<double, 3> up_x_axis = { 1, 0, 0 },
    std::array<double, 3> up_y_axis = { 0, 1, 0 },
    std::array<double, 3> up_z_axis = { 0, 0, 1 });

} // namespace omo
