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

#include "Attribute.h"
#include "Defines.h"
#include "MeshData.h"
#include "Subset.h"

#include <array>
#include <memory>
#include <string>
#include <vector>

namespace omo
{

template <Residency R>
class MeshImpl;


/// @brief Represents a mesh allocated in Host memory.

template <Residency R>
class Mesh
{
public:
    /// Constructs an empty HostMesh
    OMO_EXPORT
    Mesh();

    OMO_EXPORT //
    ~Mesh();

    /// Construct Mesh from another with any residency, copying the data from Device to Host.
    /// or vice-versa if needed.
    /// @param other_residency_mesh The source Mesh
    template <Residency S>
    OMO_EXPORT //
    explicit Mesh(const Mesh<S>& other_residency_mesh);

    /// @brief Construct from MeshData.
    /// @param mesh_data The MeshData to use.
    OMO_EXPORT
    Mesh(const MeshData<R>& mesh_data);

    /// @brief Copy constructor
    OMO_EXPORT
    Mesh(const Mesh& mesh);

    /// @brief Move constructor
    OMO_EXPORT
    Mesh(Mesh&&) noexcept;

    /// @brief Constructs a Mesh from the binary buffer provided. This buffer must have been created with
    /// the `Mesh::serialized()` method.
    /// @param buffer The byte array to read from
    /// @param error Enum holding any errors encountered during deserialization. SerializationError::None if okay.
    OMO_EXPORT
    Mesh(const std::vector<uint8_t>& buffer, SerializationError& error);

    /// @brief Copy-assignment operator
    OMO_EXPORT //
    Mesh&
    operator=(const Mesh&);

    /// @brief Move-assignment operator
    OMO_EXPORT //
    Mesh&
    operator=(Mesh&&) noexcept;

    /// @brief Equality operator
    OMO_EXPORT //
    bool
    operator==(const Mesh& rhs) const;

    /// @brief Non-equality operator
    OMO_EXPORT //
    bool
    operator!=(const Mesh& rhs) const;

    /// @return true if the mesh is empty, false otherwise
    OMO_EXPORT //
    bool
    operator!() const;

    /// @return true if the mesh has data, false otherwise
    OMO_EXPORT //
    explicit
    operator bool() const;

    /// @return The number of attributes attached to the Mesh
    [[nodiscard]]
    OMO_EXPORT //
    size_t
    numAttributes() const;

    /// @return The i:th Attribute attached to the Mesh
    [[nodiscard]]
    OMO_EXPORT //
    const Attribute<R>&
    getAttribute(size_t i) const;

    /// @brief Find a specific Attribute from a given name
    /// @param name The name of the attribute
    /// @return A Attribute matching the given name if available, otherwise an empty Attribute
    [[nodiscard]]
    OMO_EXPORT //
    const Attribute<R>&
    getAttribute(std::string_view name) const;

    /// @brief Find a specific Attribute from a given role
    /// @param role The role of the attribute
    /// @return An Attribute matching the given role if available, otherwise an empty Attribute
    [[nodiscard]]
    OMO_EXPORT //
    const Attribute<R>&
    getAttribute(Role role) const;

    /// @brief Find a specific Attribute from a given role
    /// @param role The role of the attribute
    /// @param binding The binding of the attribute
    /// @return An Attribute matching the given role and binding if available, otherwise an empty Attribute
    [[nodiscard]]
    OMO_EXPORT //
    const Attribute<R>&
    getAttribute(Role role, Binding binding) const;

    /// @brief Attaches an Attribute to the mesh.
    /// @param attr The Attribute to attach
    /// @param substitute_existing If true and the mesh already contains an attribute with the same name
    /// or the same role as `attr`, then that attribute is ejected in favor of `attr`. Otherwise `baseAttribute`
    /// is not attached.
    /// @return true if attribute is successfully attached, or false if there was an error
    OMO_EXPORT //
    bool
    attachAttribute(const Attribute<R>& attr, bool substitute_existing = false);

    /// @brief Detach an Attribute
    /// @param name The name of the attribute, if not found will do nothing
    /// @return true if the attribute was found and detached, or false otherwise
    OMO_EXPORT //
    bool
    detachAttribute(std::string_view name);

    /// @return The number of subsets attached to the Mesh
    [[nodiscard]]
    OMO_EXPORT //
    size_t
    numSubsets() const;

    /// @return The i:th Subset attached to the Mesh
    [[nodiscard]]
    OMO_EXPORT //
    const Subset<R>&
    getSubset(size_t i) const;

    /// @brief Find a specific Subset from a given name
    /// @param name The name of the subset
    /// @return A Subset matching the given name if available, otherwise an empty Subset
    [[nodiscard]]
    OMO_EXPORT //
    const Subset<R>&
    getSubset(std::string_view name) const;

    /// @brief Attach a Subset to the mesh
    /// @param subset The Subset to attach
    /// @param substitute_existing If true and the mesh already contains a subset with the same name
    /// then that attribute is ejected in favor of `subset`. Otherwise `subset` is not attached.
    /// @return true if subset is successfully attached, or false if there was an error
    OMO_EXPORT //
    bool
    attachSubset(const Subset<R>& subset, bool substitute_existing = false);

    /// @brief Detach a Subset
    /// @param name The name of the subset, if not found will do nothing
    /// @return true if the attribute was found and detached, or false otherwise
    OMO_EXPORT //
    bool
    detachSubset(std::string_view name);

    /// @return The number of vertices in the Mesh
    [[nodiscard]]
    OMO_EXPORT //
    size_t
    vertexCount() const;

    /// @return The number of faces in the Mesh
    [[nodiscard]]
    OMO_EXPORT //
    size_t
    faceCount() const;

    /// @return The number of corners in the Mesh
    [[nodiscard]]
    OMO_EXPORT //
    size_t
    cornerCount() const;

    /// @return The mesh data
    [[nodiscard]]
    OMO_EXPORT //
    MeshData<R>
    data() const;

    /// @return The localToWorld matrix, with 16 double values supplied in row-major order
    [[nodiscard]]
    OMO_EXPORT //
    std::array<double, 16>
    localToWorld() const;

    /// @return The attached attributes
    [[nodiscard]]
    OMO_EXPORT //
    const std::vector<Attribute<R>>&
    attributes() const;

    /// @return The attached subsets
    [[nodiscard]]
    OMO_EXPORT //
    const std::vector<Subset<R>>&
    subsets() const;

    /// @brief Writes the Mesh to a binary stream. This is not supported on DeviceMeshes.
    /// For Device meshes it prints an error and returns an emtpy vector.
    /// @returns A byte vector holding a binary representation of this Mesh
    /// @warning The byte stream provided is not considered a "stable" represenation and is only
    /// designed to be used with the same version of the omnimesh-ops library, on the same architecture, OS etc. This
    /// bytestream is *NOT* an interchange format. Should you wish to use this method, please discuss your usecase
    /// with the omnimesh-ops developers.
    [[nodiscard]]
    OMO_EXPORT //
    std::vector<uint8_t>
    serialized() const;

    /// @cond PRIVATE
    // For internal use only:
    explicit OMO_EXPORT
    Mesh(const MeshImpl<R>&);

    OMO_EXPORT //
    MeshImpl<R>&
    impl()
    {
        return *m_impl;
    }

    OMO_EXPORT //
    const MeshImpl<R>&
    impl() const
    {
        return *m_impl;
    }

    bool
    attachAttribute(const AttributeImpl<R>& attr_impl, bool substitute_existing = false);

    bool
    attachSubset(const SubsetImpl<R>& subset_impl, bool substitute_existing = false);

protected:
    std::unique_ptr<MeshImpl<R>> m_impl;
    /// @endcond
};

template <Residency R>
OMO_EXPORT //
std::ostream&
operator<<(std::ostream& os, const Mesh<R>& m);

using HostMesh = Mesh<Host>;
using DeviceMesh = Mesh<Device>;

} // namespace omo
