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
#include "Mesh.h"

#include <string>
#include <vector>

namespace omo
{

/// @brief Create a manifold cutting from the given Mesh
/// @param mesh The input Mesh
/// @return A manifold cutting of the input mesh, including all the attachments from the original mesh
template <Residency R>
OMO_EXPORT
Mesh<R>
manifold(const Mesh<R>& mesh);

/// @brief Create a manifold cutting from the given Mesh
/// @param mesh The input Mesh
/// @param transferred_attachments The names of the attributes to transfer to the output Mesh
/// @return A manifold cutting of the input mesh
template <Residency R>
OMO_EXPORT
Mesh<R>
manifold(const Mesh<R>& mesh, const std::vector<std::string>& transferred_attachments);

} // namespace omo
