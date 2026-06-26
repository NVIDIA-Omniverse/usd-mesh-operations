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

#include <memory>
#include <string>

namespace omo
{

template <Residency R>
class AttributeImpl;

/// @brief Represents an attribute that an be attached to a Mesh.

template <Residency R>
class Attribute
{
public:
    /// @brief Empty attribute
    OMO_EXPORT //
    static const Attribute&
    Empty();

    /// @brief Constructs an empty HostAttribute
    OMO_EXPORT
    Attribute();

    OMO_EXPORT //
    ~Attribute();

    /// @brief Constructs an Attribute from an Attribute of opposite residency, by copying data to host.
    /// @param other_residency_attr The source DeviceAttribute
    template <Residency S>
    OMO_EXPORT //
    explicit Attribute(const Attribute<S>& other_residency_attr);

    /// @brief Copy-construct an Attribute
    OMO_EXPORT
    Attribute(const Attribute&);

    /// @brief Move-construct an Attribute
    OMO_EXPORT
    Attribute(Attribute&&) noexcept;

    /// @brief Copy-assignment operator
    OMO_EXPORT //
    Attribute&
    operator=(const Attribute&);

    /// @brief Move-assignment operator
    OMO_EXPORT //
    Attribute&
    operator=(Attribute&&) noexcept;

    /// @brief Non-indexed attribute
    /// @param name The attribute name
    /// @param data_type The datatype of the raw data entries
    /// @param data The raw data pointer
    /// @param data_entry_count The number of dataType items in data (note: not a total byte size)
    /// @param binding The way that the attribute is bound to the mesh
    /// @param role The role of the attribute
    /// @param promotable True if the attribute may be promoted to PerCorner
    /// @param copy_buffer Pass true if the Attribute should make a copy of the passed in data
    OMO_EXPORT
    Attribute( //
        std::string_view name,
        DataType data_type,
        void* data,
        size_t data_entry_count,
        Binding binding,
        Role role,
        bool promotable = true,
        bool copy_buffer = true);

    /// @brief Indexed attribute
    /// @param name The attribute name
    /// @param data_type The datatype of the raw data entries
    /// @param data The raw data pointer
    /// @param data_entry_count The number of dataType items in data (note: not a total byte size)
    /// @param index The array of indices
    /// @param index_entry_count The number of indices in the array
    /// @param binding The way that the attribute is bound to the mesh
    /// @param role The role of the attribute
    /// @param promotable True if the attribute may be promoted to PerCorner
    /// @param copy_buffer Pass true if the Attribute should make a copy of the passed in data. The index is
    ///            always copied regardless.
    OMO_EXPORT
    Attribute( //
        std::string_view name,
        DataType data_type,
        void* data,
        size_t data_entry_count,
        const uint32_t* index,
        size_t index_entry_count,
        Binding binding,
        Role role,
        bool promotable = true,
        bool copy_buffer = true);

    /// @brief Used to distinguish invalid attributes when returned from an attribute returning function
    /// @return true if the Attribute name is empty, false otherwise
    OMO_EXPORT //
    bool
    operator!() const;

    /// @brief Used to distinguish invalid attributes when returned from an attribute returning function
    /// @return true if the Attribute name is nonempty, false otherwise
    OMO_EXPORT //
    explicit
    operator bool() const;

    /// @return true if the two Attributes are considered equal
    OMO_EXPORT //
    bool
    operator==(const Attribute& rhs) const;

    /// @return DataType of the Attribute
    [[nodiscard]]
    OMO_EXPORT
    DataType
    dataType() const;

    /// @return Binding of the Attribute
    [[nodiscard]]
    OMO_EXPORT
    Binding
    binding() const;

    /// @return Role of the Attribute
    [[nodiscard]]
    OMO_EXPORT //
    Role
    role() const;

    /// @return Raw pointer to the data of the Attribute, note that this may be indexed and/or using offsets.
    ///         If a flattened data ptr is desired use Attribute::resolved() to get a new Attribute, then call data()
    ///         on that.
    [[nodiscard]]
    OMO_EXPORT //
    const void*
    data() const;

    /// @return Returns the number of entries in the data buffer.
    /// @note This number matches `size()` only if the index is trivial.
    [[nodiscard]]
    OMO_EXPORT //
    size_t
    dataSize() const;

    /// @return Raw pointer to the data of the i-th entry of the Attribute
    [[nodiscard]]
    OMO_EXPORT //
    const void*
    entry(size_t i) const;

    /// @return The number of entries in data (depends on the DataType used)
    [[nodiscard]]
    OMO_EXPORT //
    size_t
    size() const;

    /// @return The byte size of a single entry in data (depends on the DataType used)
    [[nodiscard]]
    OMO_EXPORT //
    size_t
    entrySize() const;

    /// @return Raw pointer to the index of the Attribute
    [[nodiscard]]
    OMO_EXPORT //
    const uint32_t*
    index() const;

    /// @return The name of the Attribute
    [[nodiscard]]
    OMO_EXPORT //
    std::string_view
    name() const;

    /// @return True if the Attribute is promotable
    OMO_EXPORT //
    bool
    promotable() const;

    /// @brief Swaps the internal representations of this Attribute and the provided other Attribute.
    OMO_EXPORT //
    void
    swap(Attribute<R>& other);

    /// @brief Returns a new Attribute with only the n:th component of each value in this attribute
    /// @param component_index The index of the component to return
    /// @return An empty Attribute if component_index is out of range, otherwise a new Attribute
    /// with only the n:th component of each value in this attribute
    OMO_EXPORT //
    Attribute<R>
    component(size_t component_index) const;

    /// @brief Gives the Attribute a new name.
    /// @param name The new name
    OMO_EXPORT //
    void
    setName(std::string_view name);

    /// @brief Return a new Attribute that has been resolved (non-indexed, non-offset).
    OMO_EXPORT //
    Attribute<R>
    resolved() const;

    /// @cond PRIVATE

    OMO_IMPLICIT OMO_EXPORT
    Attribute(const AttributeImpl<R>&);

    OMO_EXPORT //
    AttributeImpl<R>&
    impl()
    {
        return *m_impl;
    }

    OMO_EXPORT //
    const AttributeImpl<R>&
    impl() const
    {
        return *m_impl;
    }

    /// @endcond

private:
    std::unique_ptr<AttributeImpl<R>> m_impl;
};

template <Residency R>
OMO_EXPORT //
std::ostream&
operator<<(std::ostream& os, const Attribute<R>& a);

using HostAttribute = Attribute<Host>;
using DeviceAttribute = Attribute<Device>;

/// @brief Checks that all values in the attribute are finite (no NaN or Inf).
///        Only runs for attributes whose DataType uses float or double; other types are considered valid.
/// @param attribute The attribute to check (any role and binding)
/// @return Validation with true if all float/double components are finite (or datatype is not float/double),
/// false otherwise
template <Residency R>
OMO_EXPORT //
Validation
checkAttributeFinite(const Attribute<R>& attribute);

} // namespace omo
