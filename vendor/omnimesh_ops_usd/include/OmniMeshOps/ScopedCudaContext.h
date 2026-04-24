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

typedef struct CUctx_st *CUcontext;

namespace omo
{

/// @brief Verbose flag
struct Verbose
{
    /// Verbose if true
    bool enabled;
};

/// @brief Scoped CUDA context helper class.
/// Allocates a new CUDA context that is used until the destructor is called. If a context has previously been created
/// on the thread, switches to that without creating a new context.
class ScopedCudaContext
{
public:
    /// @brief ScopedCudaContext constructor
    /// @param verbose Verbose logging toggle
    /// @param cuda_device CUDA device number to use
    /// @param heap_size CUDA heap size to allocate and use. NOTE: this only has an effect the first time a context is created for a given CUDA device.
    OMO_EXPORT
    ScopedCudaContext(omo::Verbose verbose = { false }, int cuda_device = 0, size_t heap_size = 64 * 1024 * 1024);

    OMO_EXPORT
    ~ScopedCudaContext();

private:
    CUcontext m_prev;
    bool m_verbose;
};

} // namespace omo
