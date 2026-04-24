// SPDX-FileCopyrightText: Copyright (c) 2020-2025 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
// SPDX-License-Identifier: LicenseRef-NvidiaProprietary
//
// NVIDIA CORPORATION, its affiliates and licensors retain all intellectual
// property and proprietary rights in and to this material, related
// documentation and any modifications thereto. Any use, reproduction,
// disclosure or distribution of this material and related documentation
// without an express license agreement from NVIDIA CORPORATION or
// its affiliates is strictly prohibited.

#pragma once

#include <cassert>
#include <cstdint>
#include <string>
#include <limits>
#include <stdexcept>


#ifdef _MSC_VER
#    define OMO_EXPORT __declspec(dllexport)
#else
#    define OMO_EXPORT __attribute__((visibility("default")))
#endif

#define OMO_IMPLICIT /* NOLINTNEXTLINE(modernize-use-explicit) */

namespace omo
{

enum Residency
{
    Host,
    Device
};

namespace internal
{
/// @cond PRIVATE
template <Residency R>
struct OppositeResidency
{
    static constexpr Residency value = Host;
};

template <>
struct OppositeResidency<Host>
{
    static constexpr Residency value = Device;
};
/// @endcond
} // namespace internal

/// @brief constexpr meta-function returning the opposite of a residency
///
/// For example, `opposite<Host>` evaluates to `Device`.
/// @tparam R the residency to flip
template <Residency R>
inline constexpr Residency
opposite = internal::OppositeResidency<R>::value;


/// @brief float3
struct float3
{
    float x /** pos */, y /**pos */, z /**pos */;
};

/// @brief float2
struct float2
{
    float x /**pos */, y /**pos */;
};

/// @brief Specifies how an Attribute is bound to a Mesh.
enum class Binding
{
    /// No binding defined
    None = 0,

    /// Constant single value
    Constant,

    /// One value per face
    PerFace,

    /// One value per vertex
    PerVertex,

    /// One value per corner (vertex index)
    PerCorner
};

/// @return Binding enum as text
OMO_EXPORT const char* asText(Binding binding);

/// @brief Specifies the data type used for Attribute values.
enum class DataType
{
    None,
    Bool,
    Int8,
    Int16,
    Int32,
    Int64,
    UInt8,
    UInt16,
    UInt32,
    UInt64,
    Float32,
    Float64,
    BoolArray,
    Int8Array,
    UInt8Array,
    Int16Array,
    UInt16Array,
    Int32Array,
    UInt32Array,
    Int64Array,
    UInt64Array,
    Float32Array,
    Float64Array,
    Vector2i,
    Vector2f,
    Vector2d,
    Vector3i,
    Vector3f,
    Vector3d,
    Vector4i,
    Vector4f,
    Vector4d,
    Matrix2f,
    Matrix2d,
    Matrix3f,
    Matrix3d,
    Matrix4f,
    Matrix4d,
    Vector2iArray,
    Vector2fArray,
    Vector2dArray,
    Vector3iArray,
    Vector3fArray,
    Vector3dArray,
    Vector4iArray,
    Vector4fArray,
    Vector4dArray,
    Matrix2fArray,
    Matrix2dArray,
    Matrix3fArray,
    Matrix3dArray,
    Matrix4fArray,
    Matrix4dArray,
    String,
    StringArray
};

constexpr uint32_t
getComponentCount(DataType type)
{
    switch (type)
    {
    case DataType::Bool:
    case DataType::Int8:
    case DataType::Int16:
    case DataType::Int32:
    case DataType::Int64:
    case DataType::UInt8:
    case DataType::UInt16:
    case DataType::UInt32:
    case DataType::UInt64:
    case DataType::Float32:
    case DataType::Float64:
    case DataType::BoolArray:
    case DataType::Int8Array:
    case DataType::Int16Array:
    case DataType::Int32Array:
    case DataType::Int64Array:
    case DataType::UInt8Array:
    case DataType::UInt16Array:
    case DataType::UInt32Array:
    case DataType::UInt64Array:
    case DataType::Float32Array:
    case DataType::Float64Array:
        return 1;
    case DataType::Vector2i:
    case DataType::Vector2f:
    case DataType::Vector2d:
    case DataType::Vector2iArray:
    case DataType::Vector2fArray:
    case DataType::Vector2dArray:
        return 2;
    case DataType::Vector3i:
    case DataType::Vector3f:
    case DataType::Vector3d:
    case DataType::Vector3iArray:
    case DataType::Vector3fArray:
    case DataType::Vector3dArray:
        return 3;
    case DataType::Vector4i:
    case DataType::Vector4f:
    case DataType::Vector4d:
    case DataType::Matrix2f:
    case DataType::Matrix2d:
    case DataType::Vector4iArray:
    case DataType::Vector4fArray:
    case DataType::Vector4dArray:
    case DataType::Matrix2fArray:
    case DataType::Matrix2dArray:
        return 4;
    case DataType::Matrix3f:
    case DataType::Matrix3d:
    case DataType::Matrix3fArray:
    case DataType::Matrix3dArray:
        return 9;
    case DataType::Matrix4f:
    case DataType::Matrix4d:
    case DataType::Matrix4fArray:
    case DataType::Matrix4dArray:
        return 16;
    case DataType::None:
    default:
        return 0;
    }
}

/// @return DataType enum as text
OMO_EXPORT const char* asText(DataType dataType);


/// @brief Specifies the role of the Attribute.
enum class Role
{
    None,
    Tangent,
    Normal,
    UV,
    Color,
    Material,
    Sharpness
};

inline
constexpr std::string_view
defaultAttributeName(Role role)
{
    switch (role)
    {
        default:
        case Role::None: break;
        case Role::Tangent: return "tangents";
        case Role::Normal: return "normals";
        case Role::UV: return "uvs";
        case Role::Color: return "colors";
        case Role::Material: return "materials";
        case Role::Sharpness: return "sharpness";
    }
    return "";
}

constexpr uint32_t
getElementByteSize(const DataType& t)
{
    switch (t)
    {
    case DataType::Bool:
    case DataType::Int8:
    case DataType::UInt8:
    case DataType::BoolArray:
    case DataType::Int8Array:
    case DataType::UInt8Array:
    case DataType::String: // store as array of char
    case DataType::StringArray: // stored as flattened char buffer + indices
        return 1;
    case DataType::Int16:
    case DataType::UInt16:
    case DataType::Int16Array:
    case DataType::UInt16Array:
        return 2;
    case DataType::Int32:
    case DataType::UInt32:
    case DataType::Float32:
    case DataType::Int32Array:
    case DataType::UInt32Array:
    case DataType::Float32Array:
        return 4;
    case DataType::Int64:
    case DataType::UInt64:
    case DataType::Float64:
    case DataType::Int64Array:
    case DataType::UInt64Array:
    case DataType::Float64Array:
        return 8;

    case DataType::Vector2d:
    case DataType::Vector2dArray:
        return getComponentCount(t) * getElementByteSize(DataType::Float64);
    case DataType::Vector2f:
    case DataType::Vector2fArray:
        return getComponentCount(t) * getElementByteSize(DataType::Float32);
    case DataType::Vector2i:
    case DataType::Vector2iArray:
        return getComponentCount(t) * getElementByteSize(DataType::Int32);
    case DataType::Vector3d:
    case DataType::Vector3dArray:
        return getComponentCount(t) * getElementByteSize(DataType::Float64);
    case DataType::Vector3f:
    case DataType::Vector3fArray:
        return getComponentCount(t) * getElementByteSize(DataType::Float32);
    case DataType::Vector3i:
    case DataType::Vector3iArray:
        return getComponentCount(t) * getElementByteSize(DataType::Int32);
    case DataType::Vector4d:
    case DataType::Vector4dArray:
        return getComponentCount(t) * getElementByteSize(DataType::Float64);
    case DataType::Vector4f:
    case DataType::Vector4fArray:
        return getComponentCount(t) * getElementByteSize(DataType::Float32);
    case DataType::Vector4i:
    case DataType::Vector4iArray:
        return getComponentCount(t) * getElementByteSize(DataType::Int32);
    case DataType::Matrix2d:
    case DataType::Matrix2dArray:
        return getComponentCount(t) * getElementByteSize(DataType::Float64);
    case DataType::Matrix2f:
    case DataType::Matrix2fArray:
        return getComponentCount(t) * getElementByteSize(DataType::Float32);
    case DataType::Matrix3d:
    case DataType::Matrix3dArray:
        return getComponentCount(t) * getElementByteSize(DataType::Float64);
    case DataType::Matrix3f:
    case DataType::Matrix3fArray:
        return getComponentCount(t) * getElementByteSize(DataType::Float32);
    case DataType::Matrix4d:
    case DataType::Matrix4dArray:
        return getComponentCount(t) * getElementByteSize(DataType::Float64);
    case DataType::Matrix4f:
    case DataType::Matrix4fArray:
        return getComponentCount(t) * getElementByteSize(DataType::Float32);
    default:
    case DataType::None:
        return 0;
    }
}

/// @return true if DataType stores float32 components (scalar, vector, or matrix)
constexpr bool
usesFloat32Components(DataType dt)
{
    switch (dt)
    {
    case DataType::Float32:
    case DataType::Float32Array:
    case DataType::Vector2f:
    case DataType::Vector2fArray:
    case DataType::Vector3f:
    case DataType::Vector3fArray:
    case DataType::Vector4f:
    case DataType::Vector4fArray:
    case DataType::Matrix2f:
    case DataType::Matrix2fArray:
    case DataType::Matrix3f:
    case DataType::Matrix3fArray:
    case DataType::Matrix4f:
    case DataType::Matrix4fArray:
        return true;
    default:
        return false;
    }
}

/// @return true if DataType stores float64 components (scalar, vector, or matrix)
constexpr bool
usesFloat64Components(DataType dt)
{
    switch (dt)
    {
    case DataType::Float64:
    case DataType::Float64Array:
    case DataType::Vector2d:
    case DataType::Vector2dArray:
    case DataType::Vector3d:
    case DataType::Vector3dArray:
    case DataType::Vector4d:
    case DataType::Vector4dArray:
    case DataType::Matrix2d:
    case DataType::Matrix2dArray:
    case DataType::Matrix3d:
    case DataType::Matrix3dArray:
    case DataType::Matrix4d:
    case DataType::Matrix4dArray:
        return true;
    default:
        return false;
    }
}

inline
bool
fitsInUint32(size_t v)
{
    return v <= static_cast<size_t>(std::numeric_limits<std::uint32_t>::max());
}

inline
uint32_t
toUint32OrThrow(size_t v)
{
    if (!fitsInUint32(v))
        throw std::overflow_error("size_t does not fit in uint32_t");

    return static_cast<uint32_t>(v);
}



/// @brief Specifies the subset family type.
enum class SubsetFamilyType
{
    None = 0,       /// Unknown/invalid family type
    Partition,      /// Implies that every element of the whole geometry appears exactly once in only one of the subsets
                    /// belonging to the family.
    NonOverlapping, /// An element that appears in one subset may not appear in any other subset belonging to the
                    /// family.
    Unrestricted    /// Implies that there are no restrictions w.r.t. the membership of elements in the subsets. They could
                    /// be overlapping and the union of all subsets in the family may not represent the whole.
};

/// @return SubsetFamilyType enum as text
OMO_EXPORT const char* asText(SubsetFamilyType familyType);

/// @brief Defects to fix for meshes
///
/// CoincidentVertices - merge all vertices that overlap (also @see MeshConstructionOptions:mergeVerticesTolerance)
/// DegenerateEdges - merge consecutively repeated vertex references around faces
/// DegenerateFaces - removes faces with fewer than 3 distinct vertex references
/// IsolatedVertices - removes isolated (unreferenced) vertices
/// DuplicateFaces - removes duplicate copies (up to cyclic rotation) of each face
/// Default - the default operations used for construction of Meshes
enum class Defect : unsigned int
{
    None = 0,
    CoincidentVertices = 1 << 0,
    DegenerateEdges = 1 << 1,
    DegenerateFaces = 1 << 2,
    IsolatedVertices = 1 << 3,
    DuplicateFaces = 1 << 4,
    Default = CoincidentVertices | DegenerateEdges | DegenerateFaces
};

constexpr enum Defect
operator|(const Defect& a, const Defect& b)
{
    return static_cast<Defect>(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
}

constexpr enum Defect
operator&(const Defect& a, const Defect& b)
{
    return static_cast<Defect>(static_cast<unsigned int>(a) & static_cast<unsigned int>(b));
}

constexpr enum Defect
operator~(const Defect& a)
{
    return static_cast<Defect>(~static_cast<unsigned int>(a));
}

constexpr enum Defect
operator&=(Defect& a, const Defect& b)
{
    a = a & b;
    return a;
}

constexpr enum Defect
operator|=(Defect& a, const Defect& b)
{
    a = a | b;
    return a;
}

constexpr bool
hasFlag(const Defect& self, const Defect& predicate)
{
    return (self & predicate) != Defect::None;
}

namespace meshdata
{
/// @brief For the meshdata validators & mesh construction, merge neighbor vertices around some face
struct MergeNeighbors {
    /// Merge if true
    bool enabled;
};

/// @brief For the meshdata validators & mesh construction, merge boundary vertices
struct MergeBoundaries {
    /// Merge if true
    bool enabled;
};
} // namespace meshdata


/// @brief Construction options for meshes
/// Applies the construction operations in the ops field.
struct MeshConstructionOptions
{
    /// default construct operations
    Defect fixes = Defect::Default;

    /// tolerance for CoincidentVertices (only takes effect if Defect::CoincidentVertices is applied)
    ///
    /// Two vertices are *collocated* if they have exactly the same coordinates,
    /// optionally, after dropping a certain number of precision bits or
    /// snapping (rounding) coordinates to a tolerance grid.
    ///
    /// If tolerance is not zero, the position of each point is snapped (rounded)
    /// to a grid of dimension tol for the purpose of comparison of positions.
    /// (if two vertices are closer than this they get merged)
    float mergeVerticesTolerance = 0;

    /// merge coincident boundary vertices (in Defect::CoincidentVertices)
    meshdata::MergeBoundaries mergeBoundaries = { true };

    /// merge coincident vertices that are neighbors around some face (in Defect::CoincidentVertices)
    meshdata::MergeNeighbors mergeNeighbors = { true };
};



/// @brief SerializationError when constructing a Mesh from a binary data stream. Should invoking
///Mesh(stream, parseError) yield an error that is antyhing other then None, the mesh will be invalid/empty.
enum class SerializationError
{
    None = 0,            ///< No error, the mesh has been constructed successfully
    InvalidStream = 1,   ///< The stream does not appear to hold valid data to construct a mesh
    InvalidVersion = 2,  ///< The stream was created with an older, incompatible version of this library.
    InvalidEndian = 3,   ///< The stream used an endian-ness that did not match
    StreamTruncated = 4, ///< The stream is shorter then expected. Parsing was aborted.
    UnknownError = 5     ///< Some other, not further specified error has occured.
};

/// @brief Validation result struct, evaluates to true if the check had a positive result and contains
/// an optional detailed message about the check result.
struct ValidationResult
{
    /// The result of the validation
    bool result{ false };

    /// Optional message with more details about the result
    std::string message{};

    /// A helper to be able to use a simple if statement check if the messages aren't of importance
    constexpr operator bool() const noexcept { return result; }
};

} // namespace omo
