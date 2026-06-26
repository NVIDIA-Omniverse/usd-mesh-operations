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
#include "Subset.h"

#include <array>
#include <memory>
#include <string>
#include <vector>

namespace omo
{

template <Residency R>
class Mesh;

template <Residency R>
class MeshDataImpl;


/// @brief Holds mesh data associated to a HostMesh.

template <Residency R>
class MeshData
{
public:
    /// Constructs an empty MeshData
    OMO_EXPORT
    MeshData();

    /// Constructs MeshData from the given mesh
    OMO_EXPORT //
    explicit MeshData(const Mesh<R>& mesh);

    /// @brief Copy constructor
    OMO_EXPORT
    MeshData(const MeshData&);

    /// @brief Move constructor
    OMO_EXPORT
    MeshData(MeshData&&) noexcept;

    OMO_EXPORT //
    ~MeshData();

    /// @brief Cross-residency copy constructor. Copies data from the opposite residency to construct MeshData.
    template <Residency S>
    OMO_EXPORT //
    explicit MeshData(const MeshData<S>&);

    /// @brief Copy-assignment operator
    OMO_EXPORT //
    MeshData&
    operator=(const MeshData&);

    /// @brief Move-assignment operator
    OMO_EXPORT //
    MeshData&
    operator=(MeshData&&) noexcept;

    /// @brief Construct MeshData from raw data, coordinates are assumed to be sequential array of floats values.
    ///
    /// The constructed MeshData reflects the input arrays verbatim -- no
    /// cleanup is performed. Apply `cleanup()` (from `Cleanup.h`) to the
    /// result to fix defects.
    ///
    /// @param face_sizes The number of corners per face
    /// @param face_sizes_count The number of face counts
    /// @param face_vertex_indices The indices for the points
    /// @param face_vertex_indices_count The number of indices
    /// @param vertex_coords The coordinates, or points, required to be a flat array of float values, 3 per coordinate
    /// @param vertex_coords_count The number of points. Note: not the number of float values
    /// @param attributes (Optional) array of attributes to attach
    /// @param subsets (Optional) array of subsets to attach
    /// @param local_to_world_matrix_4d (Optional) the localToWorld matrix, with 16 double values supplied in
    /// row-major order
    OMO_EXPORT
    MeshData(
        const uint32_t* face_sizes,
        size_t face_sizes_count,
        const uint32_t* face_vertex_indices,
        size_t face_vertex_indices_count,
        const float* vertex_coords,
        size_t vertex_coords_count,
        const std::vector<Attribute<R>>& attributes = {},
        const std::vector<Subset<R>>& subsets = {},
        const double* local_to_world_matrix_4d = nullptr);

    /// Construct from serialized buffer data. If deserialization fails, the MeshData will be empty.
    /// @param buffer The buffer to deserialize. Used to construct the MeshData instance.
    /// @param error If the serialized data can't be deserialized, the reason why is returned here.
    OMO_EXPORT
    MeshData(const std::vector<uint8_t>& buffer, SerializationError& error);

    /// @return The face count
    [[nodiscard]]
    OMO_EXPORT //
    size_t
    faceCount() const;

    /// @return The corner count (aka indices count, or faceVertexIndices count)
    [[nodiscard]]
    OMO_EXPORT //
    size_t
    cornerCount() const;

    /// @return The vertex count
    [[nodiscard]]
    OMO_EXPORT //
    size_t
    vertexCount() const;

    /// @return Raw pointer to the vertex coordinates (aka points) data, will always be Vector3<float32> values
    [[nodiscard]]
    OMO_EXPORT //
    const float*
    vertexCoords() const;

    /// @return Raw pointer to the corner vertex data (aka face vertex indices)
    [[nodiscard]]
    OMO_EXPORT //
    const uint32_t*
    cornerVertex() const;

    /// @return Raw pointer to the face sizes data (aka face vertex counts)
    [[nodiscard]]
    OMO_EXPORT //
    const uint32_t*
    faceSizes() const;

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
    /// @return An Attribute matching the given name if available, otherwise an empty Attribute
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

    /// @brief Writes the Mesh to a binary stream. This is only supported on Host.
    /// @returns A byte vector holding a binary representation of this MeshData
    /// @warning The byte stream provided is not considered a "stable" represenation and is only
    /// designed to be used with the same version of the omnimesh-ops library, on the same architecture, OS etc. This
    /// bytestream is *NOT* an interchange format. Should you wish to use this method, please discuss your usecase
    /// with the omni-ops developers.
    [[nodiscard]]
    OMO_EXPORT //
    std::vector<uint8_t>
    serialized() const;

    /// @cond PRIVATE
    // For internal use only:

    explicit MeshData(const MeshDataImpl<R>&);

    MeshDataImpl<R>&
    impl()
    {
        return *m_impl;
    }

    const MeshDataImpl<R>&
    impl() const
    {
        return *m_impl;
    }

    bool
    attachAttribute(const AttributeImpl<R>& attr_impl, bool substitute_existing = false);

    bool
    attachSubset(const SubsetImpl<R>& subset_impl, bool substitute_existing = false);
    /// @endcond
private:
    std::unique_ptr<MeshDataImpl<R>> m_impl;
};

template <Residency R>
OMO_EXPORT //
std::ostream&
operator<<(std::ostream& os, const MeshData<R>& m);

using HostMeshData = MeshData<Host>;
using DeviceMeshData = MeshData<Device>;

} // namespace omo
