// SPDX-FileCopyrightText: Copyright (c) 2025 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
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
#include "OmniMeshOps/Primitive.h"
#include "UsdIncludes.h"

#include <string>
#include <vector>

namespace omo::usd
{

/// @brief Represents a prim allocated in Host memory.
class HostPrim : public omo::Primitive
{
public:
    using Base = omo::Primitive;
    using Base::Base;

    OMO_EXPORT
    HostPrim(const HostPrim&) = default;

    OMO_EXPORT
    HostPrim(HostPrim&&) noexcept = default;

    OMO_EXPORT
    HostPrim&
    operator=(HostPrim&&) noexcept = default;

    OMO_EXPORT
    HostPrim&
    operator=(const HostPrim&) = default;

    OMO_EXPORT
    HostPrim(const omo::Primitive& host_prim)
        : Base(host_prim)
    {}

    /// @brief Writes the HostPrim to the provided prim path and UsdStage.
    /// @param primPath The prim path
    /// @param stage The output stage
    OMO_EXPORT
    void
    writeToUsd(const std::string& primPath, const PXR_NS::UsdStageWeakPtr& stage);

    /// @brief Return the primitive's transform as a GfMatrix4d
    OMO_EXPORT
    PXR_NS::GfMatrix4d
    gfMatrixTransform() const
    {
        return PXR_NS::GfMatrix4d(
            transform[0], transform[4], transform[8], transform[12],
            transform[1], transform[5], transform[9], transform[13],
            transform[2], transform[6], transform[10], transform[14],
            transform[3], transform[7], transform[11], transform[15]);
    }

    /// @brief Utility to remove mesh-specific attributes and non-const primvars from a gprim
    OMO_EXPORT
    static void
    removeMeshSpecificAttributesAndPrimvarsFromGprim(const std::string& primPath, const PXR_NS::UsdStageWeakPtr& stage);

    /// @brief Utility to remove gprim shape-specific (sphere, cylinder, cone, cube) attributes from a gprim
    OMO_EXPORT
    static void
    removeShapeSpecificAttributesFromGprim(const std::string& primPath, const PXR_NS::UsdStageWeakPtr& stage);

    /// @brief Utility to convert PXR_NS::UsdGeomTokens->{x,y,z} to omo::PrimitiveUpAxis
    static omo::PrimitiveUpAxis
    convertUpAxis(const PXR_NS::TfToken usdUpAxis, const omo::PrimitiveUpAxis& defaultUpAxis = PrimitiveUpAxis::Z)
    {
        if (usdUpAxis == PXR_NS::UsdGeomTokens->x)
            return PrimitiveUpAxis::X;
        else
        if (usdUpAxis == PXR_NS::UsdGeomTokens->y)
            return PrimitiveUpAxis::Y;
        else
        if (usdUpAxis == PXR_NS::UsdGeomTokens->z)
            return PrimitiveUpAxis::Z;
        else
            return defaultUpAxis;
    }
};

} // namespace omo::usd
