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
#include "Mesh.h"

#include <memory>
#include <string>
#include <vector>

namespace omo
{

/// @brief The subdivision algorithm to use.
enum class SubdivisionMethod
{
    CatmullClark, ///< Catmull-Clark subdivision.
    Loop          ///< Loop subdivision.  The mesh will be triangulated first if necessary.
};

/// @brief Data type used to describe the sharpness of creases and spikes.
typedef float sharpness_t;

/// @brief Represents a Loop or Catmull-Clark subdivision operation that operates on a Mesh.
template <Residency R>
class Subdivide
{
public:
    /// @brief Subdivide move constructor
    OMO_EXPORT
    Subdivide(Subdivide&&);

    /// @brief Constructs a Subdivide object from a given Mesh.
    /// If the mesh contains an attribute with Role::Sharpness and any integer or floating-point DataType,
    /// and either Binding::PerCorner or Binding::PerVertex, then those values will be interpreted as sharpnesses
    /// for creases (if PerCorner) or spikes (if PerVertex).
    /// @param mesh The source Mesh.
    /// @param method Which subdivision algorithm to use (see SubdivisionMethod).  Default is Catmull-Clark.
    /// @note This constructor considers *all* attributes subject to transfer.
    /// @param linear If true, new vertices are linear combination of associated source elements.  Default = false.
    OMO_EXPORT explicit
    Subdivide(const Mesh<R>& mesh,
              SubdivisionMethod method = SubdivisionMethod::CatmullClark,
              bool linear = false
             );

    /// @brief Constructs a Subdivide object from a given Mesh and crease and spike descriptions.
    /// The crease and spike descriptions will override any found in the mesh via Role::Sharpness attributes.
    /// Empty crease and spike descriptions will there remove creases and spikes defined in mesh attributes.
    /// @param mesh The source Mesh.
    /// @param crease_sides Indices of sides that are on crease edges.  (Only one per crease edge is needed.)
    /// @param crease_sharpnesses Sharpness of each crease side in the crease_sides array (see DeRose et al. 1998).
    /// If the array size does not match that of crease_sides then it must either be empty or contain one element.
    /// If empty, infinite sharpness is used for all creases.  If there is one element, that value is used for all
    /// creases.  Otherwise crease sides are ignored.
    /// @param spike_vertices Indices of vertices that are spike vertices.
    /// @param spike_sharpnesses Sharpnesses for spike_vertices, functions exactly as crease_sharpnesses does for
    /// crease_sides.
    /// @param method Which subdivision algorithm to use (see SubdivisionMethod).  Default is Catmull-Clark.
    /// @note This constructor considers *all* attributes subject to transfer.
    /// @param linear If true, new vertices are linear combination of associated source elements.  Default = false.
    OMO_EXPORT explicit
    Subdivide(const Mesh<R>& mesh,
              const std::vector<uint32_t>& crease_sides,
              const std::vector<sharpness_t>& crease_sharpnesses,
              const std::vector<uint32_t>& spike_vertices,
              const std::vector<sharpness_t>& spike_sharpnesses,
              SubdivisionMethod method = SubdivisionMethod::CatmullClark,
              bool linear = false
             );

    /// @brief Constructs a Subdivide from a given Mesh and a set of attributes to transfer.
    /// If the mesh contains an attribute with Role::Sharpness and any integer or floating-point DataType,
    /// and either Binding::PerCorner or Binding::PerVertex, then those values will be interpreted as sharpnesses
    /// for creases (if PerCorner) or spikes (if PerVertex).
    /// @param mesh The source Mesh.
    /// @param transferred_attachments A vector of attribute names to transfer, e.g {"primvars:st"}.
    /// @param method Which subdivision algorithm to use (see SubdivisionMethod).  Default is Catmull-Clark.
    /// @param linear If true, new vertices are linear combination of associated source elements.  Default = false.
    OMO_EXPORT
    Subdivide(const Mesh<R>& mesh,
              const std::vector<std::string>& transferred_attachments,
              SubdivisionMethod method = SubdivisionMethod::CatmullClark,
              bool linear = false
             );

    /// @brief Constructs a Subdivide object from a given Mesh and crease and spike descriptions and a set of
    //// attributes to transfer.
    /// The crease and spike descriptions will override any found in the mesh via Role::Sharpness attributes.
    /// Empty crease and spike descriptions will there remove creases and spikes defined in mesh attributes.
    /// @param mesh The source Mesh.
    /// @param crease_sides Indices of sides that are on crease edges.  (Only one per crease edge is needed.)
    /// @param crease_sharpnesses Sharpness of each crease side in the crease_sides array (see DeRose et al. 1998).
    /// If the array size does not match that of crease_sides then it must either be empty or contain one element.
    /// If empty, infinite sharpness is used for all creases.  If there is one element, that value is used for all
    /// creases.  Otherwise crease sides are ignored.
    /// @param spike_vertices Indices of vertices that are spike vertices.
    /// @param spike_sharpnesses Sharpnesses for spike_vertices, functions exactly as crease_sharpnesses does for
    /// crease_sides.
    /// @param transferred_attachments A vector of attribute names to transfer, e.g {"primvars:st"}.
    /// @param method Which subdivision algorithm to use (see SubdivisionMethod).  Default is Catmull-Clark.
    /// @param linear If true, new vertices are linear combination of associated source elements.  Default = false.
    OMO_EXPORT
    Subdivide(const Mesh<R>& mesh,
              const std::vector<uint32_t>& crease_sides,
              const std::vector<sharpness_t>& crease_sharpnesses,
              const std::vector<uint32_t>& spike_vertices,
              const std::vector<sharpness_t>& spike_sharpnesses,
              const std::vector<std::string>& transferred_attachments,
              SubdivisionMethod method = SubdivisionMethod::CatmullClark,
              bool linear = false
             );

    OMO_EXPORT
    ~Subdivide();

    /// @brief Executes the subdivision operation. If no source mesh was given to begin with,
    /// the return value will be an empty Mesh.
    /// @param iteration_count The number of times to subdivide.  Default = 1.
    /// @return The subdivided Mesh.
    OMO_EXPORT
    Mesh<R>
    operator() (uint32_t iteration_count = 1) const;

    /// @cond PRIVATE
    // For internal use only:
    class Impl;
    OMO_EXPORT Impl* operator->() const { return m_impl.get(); }
    OMO_EXPORT Impl& operator*() const { return *m_impl; }
    /// @endcond
private:
    std::unique_ptr<Impl> m_impl;
};

using HostSubdivide = Subdivide<Host>;
using DeviceSubdivide = Subdivide<Device>;

} // namespace omo
