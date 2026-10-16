// SPDX-FileCopyrightText: Copyright (c) 2020-2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
// SPDX-License-Identifier: LicenseRef-NvidiaProprietary
//
// NVIDIA CORPORATION, its affiliates and licensors retain all intellectual
// property and proprietary rights in and to this material, related
// documentation and any modifications thereto. Any use, reproduction,
// disclosure or distribution of this material and related documentation
// without an express license agreement from NVIDIA CORPORATION or
// its affiliates is strictly prohibited.

#ifdef _MSC_VER
    #pragma warning(push)
    #pragma warning(disable : 4244) // = Conversion from double to float / int to float
    #pragma warning(disable : 4267) // conversion from size_t to int
    #pragma warning(disable : 4305) // argument truncation from double to float
    #pragma warning(disable : 4800) // int to bool
    #pragma warning(disable : 4996) // call to std::copy with parameters that may be unsafe
    #ifndef NOMINMAX
        #define NOMINMAX // Make sure nobody #defines min or max
    #endif
    #include <Windows.h> // Include this here so we can curate
    #undef small         // defined in rpcndr.h
#elif defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    // This suppresses deprecated header warnings, which is impossible with pragmas.
    // Alternative is to specify -Wno-deprecated build option, but that disables other useful warnings too.
    #ifdef __DEPRECATED
        #define OMNI_USD_SUPPRESS_DEPRECATION_WARNINGS
        #undef __DEPRECATED
    #endif
#endif

// The bare minimum headers for forward declaring or including various types we
// need to use in our public headers, without resorting to the full UsdPCH.
#include <pxr/base/gf/math.h>
#include <pxr/base/gf/matrix2d.h>
#include <pxr/base/gf/matrix2f.h>
#include <pxr/base/gf/matrix3d.h>
#include <pxr/base/gf/matrix3f.h>
#include <pxr/base/gf/matrix4d.h>
#include <pxr/base/gf/matrix4f.h>
#include <pxr/base/js/value.h>
#include <pxr/base/vt/array.h>
#include <pxr/base/vt/dictionary.h>
#include <pxr/usd/sdf/path.h>
#include <pxr/usd/usd/common.h>
#include <pxr/usd/usd/prim.h>
#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usdGeom/gprim.h>
#include <pxr/usd/usdGeom/mesh.h>
#include <pxr/usd/usdGeom/subset.h>

#ifdef _MSC_VER
    #pragma warning(pop)
#elif defined(__GNUC__)
    #pragma GCC diagnostic pop
    #ifdef OMNI_USD_SUPPRESS_DEPRECATION_WARNINGS
        #define __DEPRECATED
        #undef OMNI_USD_SUPPRESS_DEPRECATION_WARNINGS
    #endif
#endif
