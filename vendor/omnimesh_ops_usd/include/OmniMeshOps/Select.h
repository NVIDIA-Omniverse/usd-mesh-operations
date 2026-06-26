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

#include "Attribute.h"
#include "Mesh.h"
#include "Subset.h"

#include <memory>
#include <string_view>

namespace omo
{

/// @brief Represents a Select operator that operates on a host or device mesh.

template <Residency R>
class Select
{
public:
    /// @brief Constructs a Select with the given Mesh.
    /// @param mesh The source Mesh
    /// @param attribute_name The attribute name to select on
    OMO_EXPORT
    Select(const Mesh<R>& mesh, std::string_view attribute_name);

    /// @brief Constructs a Select with the given Mesh.
    /// @param mesh The source Mesh
    /// @param attribute The attribute to select on
    OMO_EXPORT
    Select(const Mesh<R>& mesh, const Attribute<R>& attribute);

    /// @brief Constructs a Select with the given Mesh.
    /// @param mesh The source Mesh
    /// @param attribute The attribute to select on
    /// @param subset The subset to limit the selection to, only indices in this subset will be considered.
    /// @note that the subset binding must match the attribute binding, otherwise the subset will be ignored.
    OMO_EXPORT
    Select(const Mesh<R>& mesh, const Attribute<R>& attribute, const Subset<R>& subset);

    // TODO: consider adding a select that operates on an attribute + previous selection

    OMO_EXPORT //
    ~Select();

    /// @brief Executes a Select operation with the provided settings.
    /// @param min The minimum value to match
    /// @param max The maximum value to match
    /// @param min_inclusive If true, the minimum value is inclusive, otherwise it is exclusive
    /// @param max_inclusive If true, the maximum value is inclusive, otherwise it is exclusive
    /// @return A subset specifying which indices in the attribute data match the given criteria
    OMO_EXPORT //
    Subset<R>
    operator()(float min, float max, bool min_inclusive, bool max_inclusive);

    /// @brief Executes a Select operation with the provided settings.
    /// @param min The minimum value to match
    /// @param max The maximum value to match
    /// @param min_inclusive If true, the minimum value is inclusive, otherwise it is exclusive
    /// @param max_inclusive If true, the maximum value is inclusive, otherwise it is exclusive
    /// @return A subset specifying which indices in the attribute data match the given criteria
    OMO_EXPORT //
    Subset<R>
    operator()(double min, double max, bool min_inclusive, bool max_inclusive);

    /// @brief Executes a Select operation with the provided settings.
    /// @param min The minimum value to match
    /// @param max The maximum value to match
    /// @param min_inclusive If true, the minimum value is inclusive, otherwise it is exclusive
    /// @param max_inclusive If true, the maximum value is inclusive, otherwise it is exclusive
    /// @return A subset specifying which indices in the attribute data match the given criteria
    OMO_EXPORT //
    Subset<R>
    operator()(int32_t min, int32_t max, bool min_inclusive, bool max_inclusive);

private:
    class Impl;
    std::unique_ptr<Impl> m_impl;
};

using HostSelect = Select<Host>;
using DeviceSelect = Select<Device>;

} // namespace omo
