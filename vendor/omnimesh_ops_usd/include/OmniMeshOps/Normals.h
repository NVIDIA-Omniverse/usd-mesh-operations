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

namespace omo
{

template <Residency R>
class Attribute;

template <Residency R>
class Mesh;

template <Residency R>
class MeshData;

enum class WeightMode
{
    Angle,
    Area
};

/// @brief Computes corner normals for the given Mesh
/// @param mesh The Mesh to compute normals for
/// @param sharpness_angle The absolute value of the dihedral angle at an edge above which the edge is considered sharp.
///     The dihedral angle is measured in the range ]-180, 180] degrees with 0 being flat.
/// @param weight_mode The weighting method to use, either area- or angle-weighting.
/// @return An Attribute holding the corner normals
template <Residency R>
OMO_EXPORT //
Attribute<R>
cornerNormals(
    const Mesh<R>& mesh,
    float sharpness_angle = 60.f,
    WeightMode weight_mode = WeightMode::Angle);

/// @brief Computes corner normals for the given Mesh based on an existing
///     normal attribute. Finite nonzero existing entries are normalized,
///     others recomputed.
/// @param mesh The Mesh to compute normals for
/// @param existing The existing normal attribute to seed the computation from
/// @param sharpness_angle The absolute value of the dihedral angle at an edge above which the edge is considered sharp.
///     The dihedral angle is measured in the range ]-180, 180] degrees with 0 being flat.
/// @param weight_mode The weighting method to use, either area- or angle-weighting.
/// @return An Attribute holding the corner normals
template <Residency R>
OMO_EXPORT //
Attribute<R>
cornerNormals(
    const Mesh<R>& mesh,
    const Attribute<R>& existing,
    float sharpness_angle = 60.f,
    WeightMode weight_mode = WeightMode::Angle);

/// @brief Computes face normals for the given Mesh
/// @param mesh The Mesh to compute normals for
/// @return An Attribute holding the face normals
template <Residency R>
OMO_EXPORT //
Attribute<R>
faceNormals(
    const Mesh<R>& mesh);

/// @brief Computes face normals for the given Mesh based on an existing
///     normal attribute. Finite nonzero existing entries are normalized,
///     others recomputed.
/// @param mesh The Mesh to compute normals for
/// @param existing The existing normal attribute to seed the computation from
/// @return An Attribute holding the face normals
template <Residency R>
OMO_EXPORT //
Attribute<R>
faceNormals(
    const Mesh<R>& mesh,
    const Attribute<R>& existing);

/// @brief Computes vertex normals for the given Mesh
/// @param mesh The Mesh to compute normals for
/// @param weight_mode The weighting method to use, either area- or angle-weighting.
/// @return An Attribute holding the vertex normals
template <Residency R>
OMO_EXPORT //
Attribute<R>
vertexNormals(
    const Mesh<R>& mesh,
    WeightMode weight_mode = WeightMode::Angle);

/// @brief Computes vertex normals for the given Mesh based on an existing
///     normal attribute. Finite nonzero existing entries are normalized,
///     others recomputed.
/// @param mesh The Mesh to compute normals for
/// @param existing The existing normal attribute to seed the computation from
/// @param weight_mode The weighting method to use, either area- or angle-weighting.
/// @return An Attribute holding the vertex normals
template <Residency R>
OMO_EXPORT //
Attribute<R>
vertexNormals(
    const Mesh<R>& mesh,
    const Attribute<R>& existing,
    WeightMode weight_mode = WeightMode::Angle);

/// @brief Computes corner corner normals but, to reduce storage, demotes them to per vertex or per face binding
///     if this causes no change in quality (i.e. promotion back to corner normals results the same values).
///
/// @param mesh The Mesh to compute normals for
/// @param sharpness_angle The absolute value of the dihedral angle at an edge above which the edge is considered sharp.
///     The dihedral angle is measured in the range ]-180, 180] degrees with 0 being flat.
/// @param weight_mode The weighting method to use, either area- or angle-weighting.
/// @return An Attribute holding the corner normals
template <Residency R>
OMO_EXPORT //
Attribute<R>
normals(
    const Mesh<R>& mesh,
    float sharpness_angle = 60.f,
    WeightMode weight_mode = WeightMode::Angle);


enum class CheckNormalsOptions : unsigned int
{
    Strict = 0,         // Only accept unit-length vectors (within tolerance)
    AllowZeroLength = 1 // Ignore (precisely) zero vectors
};

constexpr CheckNormalsOptions
operator|(const CheckNormalsOptions& a, const CheckNormalsOptions& b)
{
    return static_cast<CheckNormalsOptions>(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
}

constexpr CheckNormalsOptions
operator&(const CheckNormalsOptions& a, const CheckNormalsOptions& b)
{
    return static_cast<CheckNormalsOptions>(static_cast<unsigned int>(a) & static_cast<unsigned int>(b));
}

constexpr bool
hasFlag(const CheckNormalsOptions& self, const CheckNormalsOptions& predicate)
{
    return (self & predicate) != CheckNormalsOptions::Strict;
}

/// @brief Checks if the passed in normals attribute has any non-unit length values
/// @param attribute The normals attribute
/// @param tolerance The tolerance to use when comparing lengths to 1.
///     Vectors with length within tolerance to 1 are accepted.
/// @param options Options for tweaking what counts as valid:
/// @return Validation will contain true if the normals attribute is valid, false otherwise
template <Residency R>
OMO_EXPORT //
Validation
checkNormalsUnitLength(
    const Attribute<R>& attribute,
    double tolerance = 1e-4,
    CheckNormalsOptions options = CheckNormalsOptions::Strict);

enum class Orientation : int
{
    RightHanded,
    LeftHanded
};

/// @brief Checks if the passed in mesh has consistent windings that agree with the passed in normals and winding order
/// @param mesh_data The MeshData to use
/// @param attribute The normals attribute
/// @param orientation The orientation that was used for the normal computation
/// @return Validation will contain true if normals and windings are consistent with the provided orientation,
/// false otherwise
template <Residency R>
OMO_EXPORT //
Validation
checkNormalsConsistentWithWinding(
    const MeshData<R>& mesh_data,
    const Attribute<R>& attribute,
    Orientation orientation = Orientation::RightHanded);

} // namespace omo
