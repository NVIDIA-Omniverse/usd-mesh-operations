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

/// @brief Represents a decimation operator that operates on a host or device mesh.

template <Residency R>
class Decimate
{
public:
    /// @brief Decimate move constructor
    OMO_EXPORT
    Decimate(Decimate&&);

    /// @brief Constructs a Decimate to decimate the given Mesh.
    /// The Mesh can be decimated multiple times, with different decimation settings.
    /// @param mesh The source Mesh
    /// @param guide_attribute_name The attribute name to use as guide for decimation, should be PerVertex<Color> or
    /// PerCorner<Normal> attribute
    OMO_EXPORT
    Decimate( //
        const Mesh<R>& mesh,
        std::string_view guide_attribute_name = {});

    /// @brief Constructs a Decimate to decimate the given Mesh.
    /// The Mesh can be decimated multiple times, with different decimation settings.
    /// @param mesh The source Mesh
    /// @param guide_attribute_name The attribute name to use as guide for decimation, should be PerVertex<Color> or
    /// PerCorner<Normal> attribute
    /// @param transferred_attachments The list of attribute and subset names to bring over from the source mesh to
    /// the decimated mesh
    OMO_EXPORT
    Decimate( //
        const Mesh<R>& mesh,
        std::string_view guide_attribute_name,
        const std::vector<std::string>& transferred_attachments);

    OMO_EXPORT ~Decimate();

    /// @brief Executes a decimation operation with the provided settings.
    /// Each invocation results in decimation of the source mesh, not of the previously decimated mesh.
    /// @param target_vertex_count Stop-condition, will stop decimating when this number of vertices is reached
    /// @param max_mean_error Stop-condition, will stop decimating when the max mean error is reached (0 to disable)
    /// @param run_parallel If true run in parallel, otherwise run a sequential algorithm
    /// @param pin_boundaries If true, maintains the arrangement of vertices on surface boundaries intact
    /// @param can_cut_and_glue If true, allows decimator to cut and glue mesh
    /// @return The decimated Mesh
    OMO_EXPORT //
    Mesh<R>
    operator()(
        size_t target_vertex_count,
        double max_mean_error,
        bool run_parallel = true,
        bool pin_boundaries = false,
        bool can_cut_and_glue = false);

    /// @brief Returns the actual mean error reached by the last decimation.
    [[nodiscard]]
    OMO_EXPORT //
    double
    meanError() const;

    class Impl;

private:
    std::unique_ptr<Impl> m_impl;
};

using HostDecimate = Decimate<Host>;
using DeviceDecimate = Decimate<Device>;

} // namespace omo
