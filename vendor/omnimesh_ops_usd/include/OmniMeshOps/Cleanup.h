// SPDX-FileCopyrightText: Copyright (c) 2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
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

#include <string>

namespace omo
{

template <Residency R>
class MeshData;

/// @brief Result of `checkClean`.
///
/// `presentDefects` lists every defect from `CleanupOptions::fixes` that
/// the check reports as present. `message` is a newline-joined,
/// human-readable description, one line per reported defect.
///
/// `operator bool()` is true when `presentDefects` is empty -- i.e.
/// when the input is already in the target subset of the requested
/// cleanup.
struct CleanupValidation
{
    /// Defects found present in the input.
    EnumSet<Defect> presentDefects;

    /// Optional, newline-joined details (one line per reported defect).
    std::string message;

    /// @brief True when no defects were reported (the mesh is clean).
    explicit constexpr
    operator bool() const noexcept
    {
        return presentDefects.empty();
    }
};

/// @brief Apply the requested mesh cleanup defects in canonical order.
///
/// Each requested defect's fixer is applied in the canonical cleanup
/// order.
///
/// @param mesh The input MeshData.
/// @param options Which defects to fix and their parameters.
/// @return A new MeshData with the requested fixes applied.
template <Residency R>
OMO_EXPORT //
MeshData<R>
cleanup(const MeshData<R>& mesh, const CleanupOptions& options = {});

/// @brief Check whether the input mesh is already in the subset that
/// `cleanup(mesh, options)` would land it in.
///
/// Reports each requested defect whose fixer would cause changes to the
/// mesh that last across the pipeline. A defect whose disruption is
/// undone by a later fixer in the same pipeline is not reported.
///
/// @param mesh The input MeshData.
/// @param options The same options that would be passed to `cleanup`.
/// @return A `CleanupValidation`. `bool(result)` is true if the input
///     is already in the target subset; otherwise `result.presentDefects`
///     names the offending defects and `result.message` describes them.
template <Residency R>
OMO_EXPORT //
CleanupValidation
checkClean(const MeshData<R>& mesh, const CleanupOptions& options = {});

} // namespace omo