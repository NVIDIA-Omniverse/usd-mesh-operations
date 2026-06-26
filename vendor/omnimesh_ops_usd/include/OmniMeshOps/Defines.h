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

#include <cassert>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>


#ifdef _MSC_VER
    #define OMO_EXPORT __declspec(dllexport)
#else
    #define OMO_EXPORT __attribute__((visibility("default")))
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
inline constexpr Residency //
    opposite = internal::OppositeResidency<R>::value;


/// @brief A type-safe bitmask built from an enum's values.
///
/// The enum's enumerators must be sequential integer indices 0, 1, 2,
/// ... — you do NOT write `1 << 0`, `1 << 1` yourself; `EnumSet`
/// computes the shifts internally. A scoped enum (`enum class`) is
/// recommended (no namespace pollution, no implicit-to-int conversion)
/// but not required.
///
/// `EnumSet<E>` lets you combine values with `|` and query membership
/// with `contains`, without losing the enum's type identity to a raw
/// integer.
///
/// @warning No bounds check is performed when constructing or testing
///     an `EnumSet` against an enum value. The caller is responsible for
///     ensuring every value passed lies in `[0, 64)`; a larger index
///     shifts past the storage width and is undefined behaviour. The
///     enum may not contain composite enumerators (values defined as
///     `A | B | C`) — sequential indices would collide with them.
///     Composites should be declared outside the enum as
///     `inline constexpr EnumSet<E>` values.
template <typename Enum>
class EnumSet
{
    static_assert(std::is_enum_v<Enum>,
                  "EnumSet<Enum>: Enum must be an enum type");

public:
    /// Underlying unsigned integer holding the bitmask (one bit per value).
    using Storage = uint64_t;

    constexpr EnumSet() = default;

    /// Construct from zero or more Enum values. With one argument this
    /// behaves as an implicit conversion (no `explicit` keyword), so
    /// `EnumSet<E> s = E::A;` works; with two or more it builds the
    /// union of all listed values.
    template <typename... Es,
              typename = std::enable_if_t<(std::is_same_v<Enum, Es> && ...)>>
    constexpr EnumSet(Es... es):
        m_bits((bitOf(es) | ... | Storage{ 0 }))
    {}

    /// @brief True if the single value @p e is present in the set.
    [[nodiscard]]
    constexpr bool
    contains(Enum e) const
    {
        return (m_bits & bitOf(e)) != 0;
    }

    /// @brief True if every value in @p other is also present in this set.
    [[nodiscard]]
    constexpr bool
    contains(EnumSet other) const
    {
        return (m_bits & other.m_bits) == other.m_bits;
    }

    /// @brief True if this set and @p other share at least one value.
    [[nodiscard]]
    constexpr bool
    intersects(EnumSet other) const
    {
        return (m_bits & other.m_bits) != 0;
    }

    /// @brief True if the set contains no values.
    [[nodiscard]]
    constexpr bool
    empty() const
    {
        return m_bits == 0;
    }

    /// @brief True if the set is non-empty.
    explicit constexpr
    operator bool() const
    {
        return !empty();
    }

    /// @brief Adds (unions in) every value from @p other.
    constexpr EnumSet&
    operator+=(EnumSet other)
    {
        m_bits |= other.m_bits;
        return *this;
    }

    /// @brief Intersects with @p other, keeping only shared values.
    constexpr EnumSet&
    operator&=(EnumSet other)
    {
        m_bits &= other.m_bits;
        return *this;
    }

    /// @brief Removes every value found in @p other.
    constexpr EnumSet&
    operator-=(EnumSet other)
    {
        m_bits &= ~other.m_bits;
        return *this;
    }

    /// @brief Union of two sets.
    friend constexpr EnumSet
    operator+(EnumSet a, EnumSet b)
    {
        a += b;
        return a;
    }

    /// @brief Set @p a with value @p b added.
    friend constexpr EnumSet
    operator+(EnumSet a, Enum b)
    {
        return a + EnumSet{ b };
    }

    /// @brief Set @p b with value @p a added.
    friend constexpr EnumSet
    operator+(Enum a, EnumSet b)
    {
        return EnumSet{ a } + b;
    }

    /// @brief Intersection of two sets.
    friend constexpr EnumSet
    operator&(EnumSet a, EnumSet b)
    {
        a &= b;
        return a;
    }

    /// @brief Set @p a with every value in @p b removed.
    friend constexpr EnumSet
    operator-(EnumSet a, EnumSet b)
    {
        a -= b;
        return a;
    }

    /// @brief True if both sets hold exactly the same values.
    friend constexpr bool
    operator==(EnumSet a, EnumSet b)
    {
        return a.m_bits == b.m_bits;
    }

    /// @brief True if the sets differ in any value.
    friend constexpr bool
    operator!=(EnumSet a, EnumSet b)
    {
        return !(a == b);
    }

private:
    static constexpr Storage
    bitOf(Enum e)
    {
        return Storage{ 1 } << static_cast<int>(e);
    }

    Storage m_bits = 0;
};

template <typename E, typename... Es>
EnumSet(E, Es...) -> EnumSet<E>;


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
OMO_EXPORT const char*
asText(Binding binding);

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
componentCount(DataType type)
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

/// @brief Returns the data type of an attribute representing a single component of one with the given data type.
constexpr DataType
componentDataType(DataType type)
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
            return type;
        case DataType::Vector2i:
        case DataType::Vector3i:
        case DataType::Vector4i:
            return DataType::Int32;
        case DataType::Vector2f:
        case DataType::Vector3f:
        case DataType::Vector4f:
        case DataType::Matrix2f:
        case DataType::Matrix3f:
        case DataType::Matrix4f:
            return DataType::Float32;
        case DataType::Vector2d:
        case DataType::Vector3d:
        case DataType::Vector4d:
        case DataType::Matrix2d:
        case DataType::Matrix3d:
        case DataType::Matrix4d:
            return DataType::Float64;
        case DataType::Vector2iArray:
        case DataType::Vector3iArray:
        case DataType::Vector4iArray:
            return DataType::Int32Array;
        case DataType::Vector2fArray:
        case DataType::Vector3fArray:
        case DataType::Vector4fArray:
        case DataType::Matrix2fArray:
        case DataType::Matrix3fArray:
        case DataType::Matrix4fArray:
            return DataType::Float32Array;
        case DataType::Vector2dArray:
        case DataType::Vector3dArray:
        case DataType::Vector4dArray:
        case DataType::Matrix2dArray:
        case DataType::Matrix3dArray:
        case DataType::Matrix4dArray:
            return DataType::Float64Array;
        default:
        case DataType::None:
            return DataType::None;
    }
}

/// @return DataType enum as text
OMO_EXPORT //
const char*
asText(DataType data_type);

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

inline constexpr std::string_view
defaultAttributeName(Role role)
{
    switch (role)
    {
        default:
        case Role::None:
            break;
        case Role::Tangent:
            return "tangents";
        case Role::Normal:
            return "normals";
        case Role::UV:
            return "uvs";
        case Role::Color:
            return "colors";
        case Role::Material:
            return "materials";
        case Role::Sharpness:
            return "sharpness";
    }
    return "";
}

constexpr uint32_t
byteSize(const DataType& data_type)
{
    switch (data_type)
    {
        case DataType::Bool:
        case DataType::Int8:
        case DataType::UInt8:
        case DataType::BoolArray:
        case DataType::Int8Array:
        case DataType::UInt8Array:
        case DataType::String:      // store as array of char
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
            return componentCount(data_type) * byteSize(DataType::Float64);
        case DataType::Vector2f:
        case DataType::Vector2fArray:
            return componentCount(data_type) * byteSize(DataType::Float32);
        case DataType::Vector2i:
        case DataType::Vector2iArray:
            return componentCount(data_type) * byteSize(DataType::Int32);
        case DataType::Vector3d:
        case DataType::Vector3dArray:
            return componentCount(data_type) * byteSize(DataType::Float64);
        case DataType::Vector3f:
        case DataType::Vector3fArray:
            return componentCount(data_type) * byteSize(DataType::Float32);
        case DataType::Vector3i:
        case DataType::Vector3iArray:
            return componentCount(data_type) * byteSize(DataType::Int32);
        case DataType::Vector4d:
        case DataType::Vector4dArray:
            return componentCount(data_type) * byteSize(DataType::Float64);
        case DataType::Vector4f:
        case DataType::Vector4fArray:
            return componentCount(data_type) * byteSize(DataType::Float32);
        case DataType::Vector4i:
        case DataType::Vector4iArray:
            return componentCount(data_type) * byteSize(DataType::Int32);
        case DataType::Matrix2d:
        case DataType::Matrix2dArray:
            return componentCount(data_type) * byteSize(DataType::Float64);
        case DataType::Matrix2f:
        case DataType::Matrix2fArray:
            return componentCount(data_type) * byteSize(DataType::Float32);
        case DataType::Matrix3d:
        case DataType::Matrix3dArray:
            return componentCount(data_type) * byteSize(DataType::Float64);
        case DataType::Matrix3f:
        case DataType::Matrix3fArray:
            return componentCount(data_type) * byteSize(DataType::Float32);
        case DataType::Matrix4d:
        case DataType::Matrix4dArray:
            return componentCount(data_type) * byteSize(DataType::Float64);
        case DataType::Matrix4f:
        case DataType::Matrix4fArray:
            return componentCount(data_type) * byteSize(DataType::Float32);
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

inline bool
fitsInUint32(size_t v)
{
    return v <= static_cast<size_t>(std::numeric_limits<std::uint32_t>::max());
}

inline uint32_t
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
    Partition,      /// Implies that every element of the whole geometry appears exactly once in only one of the
                    /// subsets belonging to the family.
    NonOverlapping, /// An element that appears in one subset may not appear in any other subset belonging to the
                    /// family.
    Unrestricted    /// Implies that there are no restrictions w.r.t. the membership of elements in the subsets.
                    /// They could be overlapping and the union of all subsets in the family may not cover the whole.
};

/// @return SubsetFamilyType enum as text
OMO_EXPORT //
const char*
asText(SubsetFamilyType family_type);

/// @brief Defects to fix for meshes.
///
/// CoincidentNeighborVertices    - vertices coincident with another vertex
///                                 on the same face
/// CoincidentBoundaryVertices    - boundary vertices coincident with another
///                                 boundary vertex on a different face
/// DegenerateEdges               - consecutively repeated vertex references
///                                 around faces
/// DegenerateFaces               - faces with fewer than 3 distinct vertex
///                                 references
/// IsolatedVertices              - vertices not referenced by any face
/// DuplicateFaces                - duplicate copies (up to cyclic rotation)
///                                 of any face
/// InconsistentlyOrientedFaces   - faces whose winding disagrees with their
///                                 neighbours along a shared edge
/// Nonmanifold                   - edges shared by more than two faces, or
///                                 vertices whose face-star is not a disk
enum class Defect
{
    CoincidentNeighborVertices,
    CoincidentBoundaryVertices,
    DegenerateEdges,
    DegenerateFaces,
    IsolatedVertices,
    DuplicateFaces,
    InconsistentlyOrientedFaces,
    Nonmanifold,
};

/// @brief Empty defect set; pass when no fixes should be applied.
inline constexpr auto noDefects = EnumSet<Defect>{};

/// @brief The defects that are manifest -- no justification for keeping them.
inline constexpr auto manifestDefects =
    EnumSet{
        Defect::CoincidentNeighborVertices,
        Defect::DegenerateEdges,
        Defect::DegenerateFaces,
    };

inline constexpr auto legacyManifestDefects =
    EnumSet{
        Defect::CoincidentNeighborVertices,
        Defect::CoincidentBoundaryVertices,
        Defect::DegenerateEdges,
        Defect::DegenerateFaces,
    };

/// @brief All defects the cleanup pipeline knows about.
inline constexpr auto allDefects =
    EnumSet{
        Defect::CoincidentNeighborVertices,
        Defect::CoincidentBoundaryVertices,
        Defect::DegenerateEdges,
        Defect::DegenerateFaces,
        Defect::IsolatedVertices,
        Defect::DuplicateFaces,
        Defect::InconsistentlyOrientedFaces,
        Defect::Nonmanifold,
    };

/// @brief The pair of coincident-vertex defects (neighbor + boundary).
/// Useful for callers that want to merge both kinds without enumerating
/// each separately.
inline constexpr auto coincidentVertexDefects =
    EnumSet{
        Defect::CoincidentNeighborVertices,
        Defect::CoincidentBoundaryVertices,
    };


/// @brief Options for the mesh cleanup pipeline.
///
/// `fixes` selects which defects to repair. The default is empty -- a
/// default-constructed `CleanupOptions` requests no cleanup. Choose a named
/// set such as `manifestDefects` (defects with no legitimate reason to keep)
/// or `allDefects` (every available repair), or build a set by enumerating
/// the individual `Defect` values you care about.
struct CleanupOptions
{
    /// Which defects to apply fixes for.
    EnumSet<Defect> fixes = noDefects;

    /// Tolerance for the coincident-vertex defects. Two vertices are
    /// coincident if their coordinates differ by less than this in the
    /// L-infinity norm. A tolerance of 0 means exact coincidence only.
    float vertexMergeTolerance = 0;
};


/// @brief SerializationError when constructing a Mesh from a binary data stream. Should invoking
/// Mesh(stream, parseError) yield an error that is anything other then None, the mesh will be invalid/empty.
enum class SerializationError
{
    None = 0,            ///< No error, the mesh has been constructed successfully
    InvalidStream = 1,   ///< The stream does not appear to hold valid data to construct a mesh
    InvalidVersion = 2,  ///< The stream was created with an older, incompatible version of this library.
    InvalidEndian = 3,   ///< The stream used an endian-ness that did not match
    StreamTruncated = 4, ///< The stream is shorter then expected. Parsing was aborted.
    UnknownError = 5     ///< Some other, not further specified error has occurred.
};

/// @brief Validation result struct, evaluates to true if the check passed and contains an
/// optional detailed message about the failure.
struct Validation
{
    /// Whether the validation passed. Default is `true` -- a default-constructed
    /// Validation represents a successful check with no message.
    bool passed{ true };

    /// Optional message; populated by the failing path of a check.
    std::string message{};

    /// A helper to be able to use a simple if statement when only the boolean matters.
    constexpr
    operator bool() const noexcept
    {
        return passed;
    }
};

} // namespace omo
