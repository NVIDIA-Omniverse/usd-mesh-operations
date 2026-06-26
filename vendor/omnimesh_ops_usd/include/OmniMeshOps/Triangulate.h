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
#include "Mesh.h"

#include <memory>
#include <string>
#include <vector>

namespace omo
{

/// @brief Represents a Triangulation operation that operates on a Mesh.
template <Residency R>
class Triangulate
{
public:
    /// @brief Triangulate move constructor
    OMO_EXPORT
    Triangulate(Triangulate&&);

    OMO_EXPORT //
    ~Triangulate();

    /// @brief Constructs a Triangulate from a given Mesh and an optional set of attributes to transfer.
    /// @param mesh The source Mesh
    /// @note This constructor considers *all* attributes subject to transfer
    OMO_EXPORT //
    explicit Triangulate(const Mesh<R>& mesh);

    /// @brief Constructs a Triangulate from a given Mesh and an optional set of attributes to transfer.
    /// @param mesh The source Mesh
    /// @param transferred_attributes A vector of attribute names to transfer, e.g {"primvars:st"}
    OMO_EXPORT
    Triangulate(const Mesh<R>& mesh, const std::vector<std::string>& transferred_attributes);

    /// @brief Executes the triangulation operation. If no source mesh was given to begin with,
    /// the return value will be an empty Mesh.
    /// @return The triangulated Mesh
    OMO_EXPORT //
    Mesh<R>
    operator()() const;

    /// @cond PRIVATE
    // For internal use only:
    class Impl;

    OMO_EXPORT Impl*
    operator->() const
    {
        return m_impl.get();
    }
    OMO_EXPORT Impl&
    operator*() const
    {
        return *m_impl;
    }
    /// @endcond
private:
    std::unique_ptr<Impl> m_impl;
};

using HostTriangulate = Triangulate<Host>;
using DeviceTriangulate = Triangulate<Device>;

} // namespace omo
