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

#include "OmniMeshOps/Attribute.h"
#include "OmniMeshOps/Defines.h"
#include "UsdIncludes.h"

#include <string>
#include <vector>

namespace omo::usd
{

/// @brief Represents a property that is tied to a HostMesh.
class HostAttribute : public omo::HostAttribute
{
public:
    using Base = omo::HostAttribute;
    using Base::Base;

    OMO_EXPORT HostAttribute(const HostAttribute& m) = default;
    OMO_EXPORT HostAttribute(HostAttribute&& m) noexcept = default;
    OMO_EXPORT HostAttribute& operator=(HostAttribute&& m) noexcept = default;
    OMO_EXPORT HostAttribute& operator=(const HostAttribute& m) = default;

    template <Residency S>
    OMO_EXPORT
    HostAttribute(const omo::Attribute<S>& m)
        : Base(m)
    {
    }

    /// @brief Construct a HostAttribute representing the provided UsdGeomPrimvar.
    /// @param primvar The primvar
    OMO_EXPORT HostAttribute(const PXR_NS::UsdGeomPrimvar& primvar);
};

} // namespace omo::usd
