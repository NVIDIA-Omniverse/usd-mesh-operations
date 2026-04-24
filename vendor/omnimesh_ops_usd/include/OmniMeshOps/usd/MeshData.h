// SPDX-FileCopyrightText: Copyright (c) 2023-2024 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
// SPDX-License-Identifier: LicenseRef-NvidiaProprietary
//
// NVIDIA CORPORATION, its affiliates and licensors retain all intellectual
// property and proprietary rights in and to this material, related
// documentation and any modifications thereto. Any use, reproduction,
// disclosure or distribution of this material and related documentation
// without an express license agreement from NVIDIA CORPORATION or
// its affiliates is strictly prohibited.
#pragma once

#include "OmniMeshOps/Defines.h"
#include "OmniMeshOps/MeshData.h"
#include "UsdIncludes.h"

#include <string>
#include <vector>

namespace omo::usd
{

/// @brief Represents a mesh allocated in Host memory.
class HostMeshData : public omo::HostMeshData
{
public:
    using Base = omo::HostMeshData;
    using Base::Base;

    OMO_EXPORT
    HostMeshData(const HostMeshData&) = default;

    OMO_EXPORT
    HostMeshData(HostMeshData&&) noexcept = default;

    OMO_EXPORT
    HostMeshData&
    operator=(HostMeshData&&) noexcept = default;

    OMO_EXPORT
    HostMeshData&
    operator=(const HostMeshData&) = default;

    OMO_EXPORT
    HostMeshData(const omo::HostMeshData& host_mesh)
        : Base(host_mesh)
    {}

    template <Residency S>
    OMO_EXPORT explicit
    HostMeshData(const omo::Mesh<S>& other_mesh)
        : Base(other_mesh)
    {}

    /// @brief Construct a HostMeshData from a UsdGeomMesh, including all primvars that exist on the UsdGeomMesh.
    ///
    /// @param usd_mesh UsdGeomMesh to represent as a HostMeshData
    /// @param opts (Optional) mesh construction options
    OMO_EXPORT
    HostMeshData(const PXR_NS::UsdGeomMesh& usd_mesh,
                 omo::MeshConstructionOptions opts = { Defect::None });

    /// @brief Construct a HostMeshData from a UsdGeomMesh, including only the primvars and subsets matching the
    /// provided list.
    /// @param usd_mesh UsdGeomMesh to represent as a HostMeshData
    /// @param primvarsAndSubsets The names of the primvars and subsets to create as attributes on the HostMeshData
    /// @param opts (Optional) mesh construction options
    OMO_EXPORT
    HostMeshData(const PXR_NS::UsdGeomMesh& usd_mesh,
                 const std::vector<std::string>& primvarsAndSubsets,
                 omo::MeshConstructionOptions opts = { Defect::None });

    /// @brief Writes the HostMeshData to the provided prim path and UsdStage.
    /// @param primPath The prim path
    /// @param stage The output stage
    OMO_EXPORT
    void
    writeToUsd(const std::string& primPath, const PXR_NS::UsdStageWeakPtr& stage);
};

} // namespace omo::usd
