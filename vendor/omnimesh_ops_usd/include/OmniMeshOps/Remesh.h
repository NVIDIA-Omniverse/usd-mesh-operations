// SPDX-FileCopyrightText: Copyright (c) 2020-2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
// SPDX-License-Identifier: LicenseRef-NvidiaProprietary
//
// NVIDIA CORPORATION, its affiliates and licensors retain all intellectual
// property and proprietary rights in and to this material, related
// documentation and any modifications thereto. Any use, reproduction,
// disclosure or distribution of this material and related documentation
// without an express license agreement from NVIDIA CORPORATION or
// its affiliates is strictly prohibited.

#pragma once

#include "Mesh.h"

#include <memory>
#include <vector>

namespace omo
{

/// @brief Represents a remeshing operator that operates on a host or device mesh.

template <Residency R>
class Remesh
{
public:
    /// @brief Remesh move constructor
    OMO_EXPORT
    Remesh(Remesh&&);

    /// @brief Constructs a Remesh to remesh the given Mesh.
    /// The Mesh can be remeshed multiple times, with different settings.
    /// @param mesh The source Mesh
    OMO_EXPORT
    Remesh(const Mesh<R>& mesh);

    /// @brief Constructs a Remesh to remesh the given Mesh.
    /// The Mesh can be remeshed multiple times, with different settings.
    /// @param mesh The source Mesh
    /// @param transferred_attributes The list of attribute names to bring over from the source mesh to the remeshed
    /// mesh
    OMO_EXPORT
    Remesh(const Mesh<R>& mesh, const std::vector<std::string>& transferred_attributes);

    OMO_EXPORT //
    ~Remesh();

    /// @brief Executes a remesh operation with the provided settings.
    /// Each invocation results in remeshing of the source mesh, not of the previously remeshed mesh.
    /// @param gradation The gradation to use, affects how many triangles are created.
    /// @param max_error Stop-condition, will stop remeshing when the max error is reached (0 to disable)
    /// @return The remeshed Mesh
    OMO_EXPORT //
    Mesh<R>
    operator()(double gradation, double max_error) const;

    class Impl;

private:
    std::unique_ptr<Impl> m_impl;
};

using HostRemesh = Remesh<Host>;
using DeviceRemesh = Remesh<Device>;

} // namespace omo
