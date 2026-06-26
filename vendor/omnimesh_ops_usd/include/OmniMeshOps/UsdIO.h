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

#include "UsdIncludes.h"
#include "MeshData.h"
#include "Mesh.h"
#include "Primitive.h"

namespace omo
{

OMO_EXPORT //
HostAttribute
importAttribute(const PXR_NS::UsdGeomPrimvar& primvar);

OMO_EXPORT //
HostSubset
importSubset(
    const PXR_NS::UsdGeomImageable& parent_geom,
    const PXR_NS::UsdGeomSubset& subset);

OMO_EXPORT //
HostMeshData
importMeshData(
    const PXR_NS::UsdGeomMesh& usd_mesh,
    CleanupOptions opts = {},
    const std::vector<std::string>& filters = {});

OMO_EXPORT //
HostMesh
importMesh(
    const PXR_NS::UsdGeomMesh& usd_mesh,
    CleanupOptions opts = {},
    const std::vector<std::string>& filters = {});

OMO_EXPORT //
void
exportUsdGeomMesh(
    const HostMeshData& mesh_data,
    const PXR_NS::UsdStageWeakPtr& stage,
    const std::string& prim_path);

OMO_EXPORT //
void
exportUsdGeomMesh(
    const HostMesh& mesh,
    const PXR_NS::UsdStageWeakPtr& stage,
    const std::string& prim_path);

/// @brief Utility to convert UsdGeomTokens->{x,y,z} to omo::PrimitiveUpAxis
OMO_EXPORT //
PrimitiveUpAxis
importUpAxis(
    const PXR_NS::TfToken& usd_up_axis,
    const PrimitiveUpAxis& default_up_axis = PrimitiveUpAxis::Z);

OMO_EXPORT //
void
exportUsdGeomGprim(
    const Primitive& prim,
    const PXR_NS::UsdStageWeakPtr& stage,
    const std::string& prim_path);


/// @brief Return the primitive's transform as a GfMatrix4d
[[nodiscard]]
OMO_EXPORT //
PXR_NS::GfMatrix4d
gfMatrixTransform(
    const Primitive& prim);

/// @brief Utility to remove mesh-specific attributes and non-const primvars from a gprim
OMO_EXPORT //
void
removeMeshSpecificAttributes(
    const PXR_NS::UsdGeomGprim& gprim);

/// @brief Utility to remove gprim shape-specific (sphere, cylinder, cone, cube) attributes from a gprim
OMO_EXPORT //
void
removeShapeSpecificAttributes(
    const PXR_NS::UsdGeomGprim& gprim);

} // namespace omo
