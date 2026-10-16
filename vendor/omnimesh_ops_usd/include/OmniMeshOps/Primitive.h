// SPDX-FileCopyrightText: Copyright (c) 2025-2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
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
#include "Mesh.h"

#include <limits>
#include <memory>
#include <string>
#include <vector>

namespace omo
{

/// @brief Integer-based enumeration of all supported primitive types.
struct PrimitiveType
{
    /// @brief The actual type enum
    enum Enum : int
    {
        Sphere = 0,  ///< Sphere (generally a translated, rotated ellipsoid)
        Cylinder,    ///< Cylinder / (generally a translated, rotated elliptical cylinder)
        Cone,        ///< Cone / (generally a translated, rotated elliptical cone)
        Cube,        ///< Cube / (generally a translated, rotated cuboid)
        Count,       ///< The number of valid types
        None = Count ///< Represents an unknown type
    };

    /// @brief A name string for each primitive type.
    /// @param type The input type, as an int.
    /// @return The name for the input type
    static std::string_view
    name(int type)
    {
        switch (type)
        {
            case Enum::Sphere:
                return "sphere";
            case Enum::Cylinder:
                return "cylinder";
            case Enum::Cone:
                return "cone";
            case Enum::Cube:
                return "cube";
            default:
                return "none";
        }
    }
};

/// @brief A mask to describe which primitive types to fit.
enum class PrimitiveTypeMask : int
{
    Sphere = (1 << int(PrimitiveType::Sphere)),
    Cylinder = (1 << int(PrimitiveType::Cylinder)),
    Cone = (1 << int(PrimitiveType::Cone)),
    Cube = (1 << int(PrimitiveType::Cube)),

    All = (1 << int(PrimitiveType::Count)) - 1 ///< All flags
};

/// @brief Up axis enumeration (local "up", defines main axis for cylinders and cones).
enum class PrimitiveUpAxis : int
{
    X = 0,
    Y = 1,
    Z = 2
};

/// @brief Representation of primitive geometry.
struct Primitive
{
    std::array<double, 16> transform; ///< Transformation matrix, flattened in row-major format.
    PrimitiveType::Enum type;         ///< The primitive type represented.
};

/// @brief ABC to abstract data gathering from om::primitive::Fitter
/// Keeping the interface and implementation separate in case I want to add more evaluators later.
struct PrimitiveFitEvaluator
{
    /// @brief Primitive fit error stats interface
    struct FitResult
    {
        virtual ~FitResult() = default;
        /// @brief relative volume error metric for the Primitive
        [[nodiscard]]
        virtual double volumeError(PrimitiveType::Enum) const = 0;
        /// @brief RMS vertex error metric for the Primitive
        [[nodiscard]]
        virtual double vertexError(PrimitiveType::Enum) const = 0;
        /// @brief number of missing endcaps for the Primitive
        [[nodiscard]]
        virtual uint32_t missingEndcaps(PrimitiveType::Enum) const = 0;
    };

    virtual ~PrimitiveFitEvaluator() = default;

    /// @brief Return the best fit given the passed in arguments
    [[nodiscard]]
    virtual PrimitiveType::Enum
    bestFit(const FitResult& fit_result,
            double volume_tol,
            double vertex_tol,
            bool allow_missing_endcaps,
            PrimitiveTypeMask type_mask) const = 0;
};

/// @brief The default PrimitiveFitEvaluator
struct DefaultPrimitiveFitEvaluator:
    public PrimitiveFitEvaluator
{
    /// @brief The implementation of the default primitive fit evaluator
    [[nodiscard]]
    PrimitiveType::Enum
    bestFit(
        const FitResult& fit_result,
        double volume_tol,
        double vertex_tol,
        bool allow_missing_endcaps,
        PrimitiveTypeMask type_mask) const override
    {
        double least_error = std::numeric_limits<double>::infinity();
        PrimitiveType::Enum best_fit_type = PrimitiveType::None;

        for (int type_i = 0; type_i < int(PrimitiveType::Count); ++type_i)
        {
            if (!((int(type_mask) >> type_i) & 1))
                continue;

            const auto type = PrimitiveType::Enum(type_i);

            if (!allow_missing_endcaps && fit_result.missingEndcaps(type) > 0)
                continue;

            const double volume_error = fit_result.volumeError(type);
            const double vertex_error = fit_result.vertexError(type);

            if (volume_error > volume_tol || vertex_error > vertex_tol)
                continue;

            if (volume_error < least_error)
            {
                best_fit_type = type;
                least_error = volume_error;
            }
        }

        return best_fit_type;
    }
};

/// @brief Represents a primitive fit operation that requires a Mesh input.
template <Residency R>
class PrimitiveFit
{
public:
    /// @brief PrimitiveFit move constructor
    OMO_EXPORT
    PrimitiveFit(PrimitiveFit&&) noexcept;

    /// @brief Constructs a PrimitiveFit object from a given Mesh.
    /// @param mesh The source Mesh.
    OMO_EXPORT //
    explicit PrimitiveFit(const Mesh<R>& mesh);

    OMO_EXPORT //
    ~PrimitiveFit();

    /// @brief Executes the primitive fit operation.
    /// @param up Primitive's local frame "up.""  Default is Z.
    /// @param type_mask A bit mask describing which primitive types to fit.  Default is all types.
    /// @param volume_tol The relative volume error tolerance.  Default = 0.01.
    /// @param vertex_tol The relative RMS vertex distance tolerance.  Default = 0.01.
    /// @param allow_negative_volume If true, fit primitives with inward-facing normals.  Default is true.
    /// @param allow_missing_endcaps If true, fit primitives with no endcaps or a base.  Default is true.
    /// @return The best-fit primitive type (Primitive::PrimitiveType::None if none are within tolerance.)
    OMO_EXPORT
    Primitive
    operator()(
        PrimitiveUpAxis up = PrimitiveUpAxis::Z,
        int type_mask = int(PrimitiveTypeMask::All),
        double volume_tol = 0.01,
        double vertex_tol = 0.01,
        bool allow_negative_volume = true,
        bool allow_missing_endcaps = true) const;

    /// @cond PRIVATE
    // For internal use only:
    class Impl;

    OMO_EXPORT //
    Impl*
    operator->() const
    {
        return m_impl.get();
    }

    OMO_EXPORT //
    Impl&
    operator*() const
    {
        return *m_impl;
    }
    /// @endcond

private:
    std::unique_ptr<Impl> m_impl;
};

using HostPrimitiveFit = PrimitiveFit<Host>;
using DevicePrimitiveFit = PrimitiveFit<Device>;

/// @brief Parameters used to create a mesh to approximate a primitive type
struct PrimitiveMeshParameters
{
    uint32_t n_radial; ///< # of divisions around axis, must be >= 3.  Ignored for cubes.
    uint32_t n_axial;  ///< # of latitude divisions for spheres, # of lengthwise sections otherwise.  Ignored for cubes.
    bool capped;       ///< For cylinders and cones, whether or not to add endcaps or a base, respectively.
};

/// @brief Wrapper to build PrimitiveMeshParameters for a sphere with reasonable default values.
struct SphereMeshParameters: public PrimitiveMeshParameters
{
    /// @brief SphereMeshParameters constructor.
    /// @param n_longitude The number of divisions around the sphere's equator.  Must be at least 3.
    /// @param n_latitude The number of divisions from pole to pole.  Must be at least two.
    SphereMeshParameters(uint32_t n_longitude = 32, uint32_t n_latitude = 16):
        PrimitiveMeshParameters{ n_longitude, n_latitude, false }
    {}
};

/// @brief Wrapper to build PrimitiveMeshParameters for a cylinder with reasonable default values.
struct CylinderMeshParameters: public PrimitiveMeshParameters
{
    /// @brief CylinderMeshParameters constructor.
    /// @param n_radial The number of divisions around the cylinder's wall.  Must be at least 3.
    /// @param n_length The number of divisions from end to end.  Must be at least one.
    /// @param endcaps If true, add endcap faces to close the mesh.
    CylinderMeshParameters(uint32_t n_radial = 32, uint32_t n_length = 1, bool endcaps = true):
        PrimitiveMeshParameters{ n_radial, n_length, endcaps }
    {}
};

/// @brief Wrapper to build PrimitiveMeshParameters for a cone with reasonable default values.
struct ConeMeshParameters: public PrimitiveMeshParameters
{
    /// @brief ConeMeshParameters constructor.
    /// @param n_radial The number of divisions around the cone's side.  Must be at least 3.
    /// @param n_length The number of divisions from end to end.  Must be at least one.
    /// @param base If true, add base faces to close the mesh.
    ConeMeshParameters(uint32_t n_radial = 64, uint32_t n_length = 3, bool base = true):
        PrimitiveMeshParameters{ n_radial, n_length, base }
    {}
};

/// @brief Wrapper to build PrimitiveMeshParameters for a cube.  Only the type parameter is used for cube construction.
struct CubeMeshParameters: public PrimitiveMeshParameters
{
    /// @brief CubeMeshParameters constructor.
    CubeMeshParameters():
        PrimitiveMeshParameters{ 0, 0, false }
    {}
};

/// @brief Create a mesh to approximate a primitive type.
/// @param type The primitive type to approximate.
/// @param parameters Describes the tessellation parameters and endcap/base inclusion when appropriate.
/// @param baked_xform Transform to modify the mesh's point and normal buffers.
/// This is a flattened row-major 4x4 homogeneous local-to-world transform.  This is used to transform vertex
/// coordinates, while the inverse transpose of the 3x3 ("linear" part) is used to transform normals.
/// @param l2w_xform Transform to include with returned mesh, which can be used in a scene graph.
/// This is a flattened row-major 4x4 homogeneous local-to-world transform.  This is used to transform vertex
/// coordinates, while the inverse transpose of the 3x3 ("linear" part) is used to transform normals.
/// @return Mesh that approximates the primitive type.
template <Residency R>
OMO_EXPORT Mesh<R>
createPrimitiveMesh(
    PrimitiveType::Enum type,
    const PrimitiveMeshParameters& parameters,
    const std::array<double, 16>& baked_xform =
        std::array<double, 16>{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },
    const std::array<double, 16>& l2w_xform =
        std::array<double, 16>{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 });

} // namespace omo
