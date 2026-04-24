// SPDX-FileCopyrightText: Copyright (c) 2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
// SPDX-License-Identifier: LicenseRef-NvidiaProprietary
//
// NVIDIA CORPORATION, its affiliates and licensors retain all intellectual
// property and proprietary rights in and to this material, related
// documentation and any modifications thereto. Any use, reproduction,
// disclosure or distribution of this material and related documentation
// without an express license agreement from NVIDIA CORPORATION or
// its affiliates is strictly prohibited.
#pragma once

#include <cstdint>
#include <vector>

#include "autouv/autouv_options.h"

namespace autouv
{

  struct ParameterizeResultMinimal
  {
    std::vector<double> coords;      // [nC*2] UV coords, row-major [u0,v0, u1,v1, ...]
    std::vector<int32_t> coord_inds; // [nF*3] per-corner UV indices into coords, row-major
    std::vector<int32_t> patch_ids;  // [nF] per-face patch IDs
  };

  // ====================================================
  // == High-level parameterize function
  // ====================================================
  //
  // This mimics the primary API in patch_parameterize_pipeline.h, but does not expose any Eigen headers, std library only.

  AUTOUV_EXPORT
  ParameterizeResultMinimal patch_parameterize_pipeline_minimal(
      const std::vector<double>& V,          // [nV*3] vertex positions, row-major [x0,y0,z0, ...]
      const std::vector<int32_t>& F,         // [nF*3] face vertex indices, row-major
      const std::vector<bool>& edge_is_real, // [nF*3] per-halfedge real-edge flags for triangulated polygons
      const ParameterizeOptions& options);

  AUTOUV_EXPORT
  ParameterizeResultMinimal patch_parameterize_pipeline_minimal(
      const std::vector<double>& V,  // [nV*3] vertex positions, row-major [x0,y0,z0, ...]
      const std::vector<int32_t>& F, // [nF*3] face vertex indices, row-major
      const ParameterizeOptions& options);

} // namespace autouv
