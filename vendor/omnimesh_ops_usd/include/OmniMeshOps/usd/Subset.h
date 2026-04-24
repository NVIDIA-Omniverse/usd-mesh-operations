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

#include "OmniMeshOps/Defines.h"
#include "OmniMeshOps/Subset.h"
#include "UsdIncludes.h"

#include <string>
#include <vector>

namespace omo::usd
{

class HostSubset : public omo::HostSubset
{
public:
    using Base = omo::HostSubset;
    using Base::Base;

    OMO_EXPORT HostSubset(const HostSubset& m) = default;
    OMO_EXPORT HostSubset(HostSubset&& m) noexcept = default;
    OMO_EXPORT HostSubset& operator=(HostSubset&& m) noexcept = default;
    OMO_EXPORT HostSubset& operator=(const HostSubset& m) = default;

    OMO_EXPORT
    HostSubset(omo::HostSubset&& m)
        : Base(m)
    {}

    template <Residency S>
    OMO_EXPORT
    HostSubset(const omo::Subset<S>& m)
        : Base(m)
    {}

    OMO_EXPORT HostSubset(const PXR_NS::UsdGeomImageable& parentGeom, const PXR_NS::UsdGeomSubset& subset);
};

using DeviceSubset = omo::DeviceSubset;

} // namespace omo::usd
