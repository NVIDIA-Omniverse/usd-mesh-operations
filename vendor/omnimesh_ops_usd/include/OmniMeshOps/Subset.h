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

#include "Defines.h"

#include <memory>
#include <string>

namespace omo
{

template <Residency R>
class SubsetImpl;

/// @brief Represents a subset property that is tied to a Mesh.

template <Residency R>
class Subset
{
public:
    /// Empty subset
    OMO_EXPORT static
    const Subset&
    Empty();

    /// Constructs an empty Subset
    OMO_EXPORT
    Subset();

    OMO_EXPORT
    ~Subset();

    /// @brief Constructs a Subset from a subset with opposite residency, by copying data.
    template <Residency S>
    OMO_EXPORT explicit
    Subset(const Subset<S>&);

    /// @brief Copy constructor
    OMO_EXPORT
    Subset(const Subset&);

    /// @brief Move constructor
    OMO_EXPORT
    Subset(Subset&&) noexcept;

    /// @brief Copy-assignment operator
    OMO_EXPORT
    Subset&
    operator=(const Subset&);

    /// @brief Move-assignment operator
    OMO_EXPORT
    Subset&
    operator=(Subset&&) noexcept;

    /// \param name The Subset name
    /// \param data The raw data pointer
    /// \param dataCount The number of dataType items in data (note: not a total byte size)
    /// \param binding The way that the Subset is bound to the mesh
    /// \param familyName The name of the subset family
    /// \param familyType The subset family type
    /// \param copy_buffer Pass true if the Subset should make a copy of the passed in data
    OMO_EXPORT
    Subset(std::string_view name,
           const uint32_t* data,
           size_t dataCount,
           Binding binding,
           std::string_view familyName,
           SubsetFamilyType familyType,
           bool copy_buffer = true);

    /// @return true if the Subset is empty, false otherwise
    OMO_EXPORT
    bool
    operator!() const;

    /// @return true if the Subset has data, false otherwise
    OMO_EXPORT explicit
    operator bool() const;

    /// @return true if the two Subsets are considered equal
    OMO_EXPORT
    bool
    operator==(const Subset& rhs) const;

    /// @return Binding of the Subset
    [[nodiscard]] OMO_EXPORT
    Binding
    binding() const;

    /// @return Family type of the Subset
    [[nodiscard]] OMO_EXPORT
    SubsetFamilyType
    familyType() const;

    /// @return Family name that the Subset belongs to
    [[nodiscard]] OMO_EXPORT
    std::string_view
    familyName() const;

    /// @return Raw pointer to the data of the Subset
    [[nodiscard]] OMO_EXPORT
    const uint32_t*
    data() const;

    /// @return The number of entries in data (depends on the DataType used)
    [[nodiscard]] OMO_EXPORT
    size_t
    size() const;

    /// @return The name of the Subset
    [[nodiscard]] OMO_EXPORT
    std::string_view
    name() const;

    /// @cond PRIVATE
    // For internal use only:

    explicit OMO_EXPORT
    Subset(const SubsetImpl<R>&);

    OMO_EXPORT
    SubsetImpl<R>&
    impl()
    { return *m_impl; }

    OMO_EXPORT
    const SubsetImpl<R>&
    impl() const
    { return *m_impl; }

    /// @endcond

private:
    std::unique_ptr<SubsetImpl<R>> m_impl;
};

template <Residency R>
OMO_EXPORT std::ostream& operator<<(std::ostream& os, const Subset<R>& a);

using HostSubset = Subset<Host>;
using DeviceSubset = Subset<Device>;

} // namespace omo
