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
#include "Normals.h"

#include <string>
#include <vector>

namespace omo
{

template <Residency R>
class MeshData;

/// @brief Specifies Coorient options.
enum class CoorientOptions
{
    None,                      ///< No special option, use default handling.
    FlipNormalsOfReversedFaces ///< Flip the normals of any face that coorient reverses
};

/// @brief Coorient the faces of the given MeshData.
///
/// If FlipNormalsOfReversedFaces is used, then all PerVertex Attributes with role Normal will be promoted to
/// PerCorner and flipped where faces are reversed; other attributes are copied according to their bindings.
///
/// @param mesh_data The MeshData to process
/// @param options If FlipNormalsOfReversedFaces is passed (default), any face that gets cooriented will also
/// flip the corresponding normal(s)
/// @return A new MeshData with faces cooriented
template <Residency R>
OMO_EXPORT //
MeshData<R>
coorient(
    const MeshData<R>& mesh_data,
    const CoorientOptions& options = CoorientOptions::FlipNormalsOfReversedFaces);

/// @brief Coorient the faces of the given MeshData.
///
/// If FlipNormalsOfReversedFaces is used, then all PerVertex Attributes with role Normal will be promoted to
/// PerCorner and flipped where faces are reversed; other attributes are copied according to their bindings.
///
/// @param mesh_data The MeshData to process
/// @param transferred_attachments The names of the attributes/subsets to transfer to the output MeshData
/// @param options If FlipNormalsOfReversedFaces is passed (default), any face that gets cooriented will
/// also flip the corresponding normal(s)
/// @return A new MeshData with faces cooriented
template <Residency R>
OMO_EXPORT //
MeshData<R>
coorient(
    const MeshData<R>& mesh_data,
    const std::vector<std::string>& transferred_attachments,
    const CoorientOptions& options = CoorientOptions::FlipNormalsOfReversedFaces);

} // namespace omo
