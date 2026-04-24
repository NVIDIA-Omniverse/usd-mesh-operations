// SPDX-FileCopyrightText: Copyright (c) 2023-2025 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
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
#include "Vec.h"

#include <memory>
#include <vector>

namespace MeshTools
{

/**
 * @brief Holds an array of 32-bit integers.
 */
class Int32Array
{
public:
    MESHTOOLS_EXPORT Int32Array(size_t size = 0, int value = 0);
    MESHTOOLS_EXPORT Int32Array(size_t size, const int* data);
    MESHTOOLS_EXPORT Int32Array(const Int32Array& other);

    MESHTOOLS_EXPORT ~Int32Array();

    MESHTOOLS_EXPORT size_t size() const;
    MESHTOOLS_EXPORT int* data();
    MESHTOOLS_EXPORT bool empty() const;

    MESHTOOLS_EXPORT void clear();
    MESHTOOLS_EXPORT void resize(size_t size, int value = 0);
    MESHTOOLS_EXPORT void copy(size_t size, const int* data);
    MESHTOOLS_EXPORT void copyTo(std::vector<int>& vec) const;

    MESHTOOLS_EXPORT int& operator[](size_t index);
    MESHTOOLS_EXPORT const int& operator[](size_t index) const;

    MESHTOOLS_EXPORT void push_back(int value);


    /*! \cond PRIVATE */
protected:
    class Int32ArrayImpl;
    MESHTOOLS_EXPORT Int32Array(const Int32ArrayImpl&);
    std::unique_ptr<Int32ArrayImpl> m_impl;
    /*! \endcond */
};

/**
 * @brief Holds an array of 32-bit floating point numbers.
 */
class Float32Array
{
public:
    MESHTOOLS_EXPORT Float32Array(size_t size = 0, float value = 0.0f);
    MESHTOOLS_EXPORT Float32Array(size_t size, const float* data);
    MESHTOOLS_EXPORT Float32Array(const Float32Array& other);

    MESHTOOLS_EXPORT ~Float32Array();

    MESHTOOLS_EXPORT size_t size() const;
    MESHTOOLS_EXPORT float* data();
    MESHTOOLS_EXPORT bool empty() const;

    MESHTOOLS_EXPORT void clear();
    MESHTOOLS_EXPORT void resize(size_t size, float value = 0.0f);
    MESHTOOLS_EXPORT void copy(size_t size, const float* data);
    MESHTOOLS_EXPORT void copyTo(std::vector<float>& vec) const;

    MESHTOOLS_EXPORT float& operator[](size_t index);
    MESHTOOLS_EXPORT const float& operator[](size_t index) const;

    MESHTOOLS_EXPORT void push_back(float value);

    /*! \cond PRIVATE */
protected:
    class Float32ArrayImpl;
    MESHTOOLS_EXPORT Float32Array(const Float32ArrayImpl&);
    std::unique_ptr<Float32ArrayImpl> m_impl;
    /*! \endcond */
};

/**
 * @brief Holds an array of Vec3 values.
 */
class Vec3Array
{
public:
    MESHTOOLS_EXPORT Vec3Array(size_t size = 0, const Vec3& value = Vec3(Zero));
    MESHTOOLS_EXPORT Vec3Array(size_t size, const Vec3* data);
    MESHTOOLS_EXPORT Vec3Array(const Vec3Array& other);

    MESHTOOLS_EXPORT ~Vec3Array();

    MESHTOOLS_EXPORT size_t size() const;
    MESHTOOLS_EXPORT Vec3* data();
    MESHTOOLS_EXPORT bool empty() const;

    MESHTOOLS_EXPORT void clear();
    MESHTOOLS_EXPORT void resize(size_t size, const Vec3& value = Vec3(Zero));
    MESHTOOLS_EXPORT void copy(size_t size, const Vec3* data);
    MESHTOOLS_EXPORT void copyTo(std::vector<Vec3>& vec) const;

    MESHTOOLS_EXPORT Vec3& operator[](size_t index);
    MESHTOOLS_EXPORT const Vec3& operator[](size_t index) const;

    MESHTOOLS_EXPORT void push_back(const Vec3& value);

    /*! \cond PRIVATE */
protected:
    class Vec3ArrayImpl;
    MESHTOOLS_EXPORT Vec3Array(const Vec3ArrayImpl&);
    std::unique_ptr<Vec3ArrayImpl> m_impl;
    /*! \endcond */
};

} // namespace MeshTools
