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

#include "Defines.h"
#include "Mesh.h"

#include <memory>
#include <string>
#include <vector>
#include <limits>

namespace omo
{

/// @brief Transfer attributes from one mesh to another
template <Residency R>
class MeshAttributeXfer
{
public:
    /// @brief MeshAttributeXfer move constructor
    OMO_EXPORT
    MeshAttributeXfer(MeshAttributeXfer&&);

    /// @brief Constructor
    /// @param src_mesh The source Mesh.  All attributes (with the possible exception of normals)
    /// will be transferred to a destination mesh with the () operator.
    /// @param transfer_normals If true, and the source mesh has attributes with role Role::Normal
    /// with data type DataType::Vector3fArray or DataType::Vector3dArray, then then they will be
    /// interpolated, transformed, and added to the dst_mesh.  Otherwise attributes with role
    /// Role::Normal will be ignored.
    OMO_EXPORT explicit
    MeshAttributeXfer(const Mesh<R>& src_mesh, bool transfer_normals = false);

    /// @brief Constructor
    /// @param src_mesh The source Mesh.  All attributes in the supplied transferred_attributes array
    /// will be transferred to a destination mesh with the () operator.
    /// @param transferred_attributes A vector of attribute names to transfer, e.g {"primvars:st"}.
    OMO_EXPORT explicit
    MeshAttributeXfer(const Mesh<R>& src_mesh, const std::vector<std::string>& transferred_attributes);

    OMO_EXPORT
    ~MeshAttributeXfer();

    /// @brief The main operation, attributes from src_mesh (set in the constructor)
    /// will be transferred and interpolated appropriately to dst_mesh.  Note, only attributes
    /// with floating-point types are interpolated.  For other types, there is no transfer of attribute
    /// from source to destination mesh.  Any corresponding attribute in the destination mesh will
    /// be copied into the return value directly.
    /// @param dst_mesh The mesh to receive the transferred attributes.  This mesh will
    /// not be modified; instead, a copy is modified and returned by the function.
    /// @param transform A transform from the local space of dst_mesh to the local_space
    /// of src_mesh, which places the two in such a way that nearby features correspond
    /// in a way that makes sense for the transferred attributes.
    /// transfer_normals Whether or not to include vertex normals among transferred
    /// attributes (assuming src_mesh has them).  This allows other tools' normal generation
    /// to be preserved.
    /// @return A copy of dst_mesh with attributes added (or replaced) from src_mesh.
    OMO_EXPORT
    Mesh<R>
    operator()(const Mesh<R>& dst_mesh, const std::array<double, 16>& transform =
        std::array<double, 16>{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 });

    /// @cond PRIVATE
    // For internal use only:
    class Impl;
    OMO_EXPORT Impl* operator->() const { return m_impl.get(); }
    OMO_EXPORT Impl& operator*() const { return *m_impl; }
    /// @endcond

private:
    std::unique_ptr<Impl> m_impl;
};

using HostMeshAttributeXfer = MeshAttributeXfer<Host>;
using DeviceMeshAttributeXfer = MeshAttributeXfer<Device>;

} // namespace omo
