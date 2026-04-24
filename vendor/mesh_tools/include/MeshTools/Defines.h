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

#include <cstddef>
#include <vector>

#ifdef _MSC_VER
#    define MESHTOOLS_EXPORT __declspec(dllexport)
#else
#    define MESHTOOLS_EXPORT __attribute__((visibility("default")))
#endif

namespace MeshTools
{

/**
 * @brief Callback type for log output (used by ClashDetector, NavigationMesh, and internal LogHandler).
 * @param message The message to log (owned by caller; do not store).
 * @param userData User data passed to setLogCallbackFunction.
 */
using LogCallbackFunction = void (*)(const char* message, void* userData);

enum class MESHTOOLS_EXPORT Granularity
{
    /// Per face visibility
    FACE,

    // Per island visibility
    ISLAND,

    // Per mesh visibility
    MESH,
};


/**
 * @brief Defines a static buffer
 */
template <typename T>
struct StaticBuffer
{
    StaticBuffer(size_t size = 0, const T* data = nullptr) : size(size), data(data)
    {
    }
    StaticBuffer(const std::vector<T>& vec) : size(vec.size()), data(vec.data())
    {
    }
    void copyTo(std::vector<T>& vec) const
    {
        vec.assign(data, data + size);
    }
    const T operator[](size_t i) const
    {
        return data[i];
    }
    size_t size;
    const T* data;
};

} // namespace MeshTools
