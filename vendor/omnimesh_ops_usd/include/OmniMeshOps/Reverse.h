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
#include <vector>

namespace omo
{

template <Residency R>
class MeshData;

enum class ReverseOptions
{
    None,           ///< No special option, use default handling.
    FlipNormals     ///< Flip normals in addition to reversing faces.
};

/// @brief Reverse the faces of the given MeshData
/// @param mesh_data The MeshData to reverse
/// @param options Optional behavior (e.g. FlipNormals)
/// @return A new MeshData with faces reversed
template <Residency R>
OMO_EXPORT
MeshData<R>
reverse(
    const MeshData<R>& mesh_data,
    const ReverseOptions& options = ReverseOptions::None
);

/// @brief Reverse the faces of the given MeshData
/// @param mesh_data The MeshData to reverse
/// @param transferred_attachments The names of the attributes/subsets to transfer to the output MeshData
/// @param options Optional behavior (e.g. FlipNormals)
/// @return A new MeshData with faces reversed
template <Residency R>
OMO_EXPORT
MeshData<R>
reverse(
    const MeshData<R>& mesh_data,
    const std::vector<std::string>& transferred_attachments,
    const ReverseOptions& options = ReverseOptions::None
);

} // namespace omo
