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
#include "Vec.h"

#include <memory>
#include <string>
#include <vector>

namespace MeshTools
{

/**
 * @brief Decribes the role of an attribute.
 *
 */
enum class AttributeRole
{
    UNKNOWN,
    NORMALS,
    TANGENTS,
    VELOCITIES,
    ACCELERATIONS,
    MEAN_CURVATURES,
    PRINCIPAL_CURVATURES,
    UVS,
    UVS_3D,
    COLORS,
    MATERIAL,
    PRIM_VAR,
    NUM_ROLES
};

/**
 * @brief Decribes how an attribute is related to the mesh.
 *
 */
enum class AttributeBinding
{
    UNKNOWN,
    FACE_VARYING,
    VERTEX_VARYING,
    PER_FACE,
    CONSTANT,
    NUM_BINDINGS
};

/**
 * @brief Describes the type of the components of an attribute.
 */
enum class AttributeComponentType
{
    FLOAT,
    INT,
};

/**
 * @brief Represents an attribute of a mesh containing a vector of values.
 */
class Attribute
{
public:
    MESHTOOLS_EXPORT Attribute(const std::string& name,
                               AttributeRole role,
                               AttributeBinding binding,
                               int elementSize,
                               size_t numElements,
                               const float* data = nullptr);

    MESHTOOLS_EXPORT Attribute(const std::string& name,
                               AttributeRole role,
                               AttributeBinding binding,
                               size_t numElements,
                               const int* data = nullptr);

    MESHTOOLS_EXPORT Attribute(const Attribute& other);

    MESHTOOLS_EXPORT ~Attribute();

    MESHTOOLS_EXPORT void resize(size_t numElements);

    MESHTOOLS_EXPORT void pushBack(const Vec& v);

    MESHTOOLS_EXPORT void pushBack(int val);

    MESHTOOLS_EXPORT bool getElement(size_t index, Vec& v) const;

    MESHTOOLS_EXPORT bool setElement(size_t index, const Vec& v);

    MESHTOOLS_EXPORT bool getElement(size_t index, int& val) const;

    MESHTOOLS_EXPORT bool setElement(size_t index, int val);

    MESHTOOLS_EXPORT bool isEqual(size_t index0, size_t index1) const;

    MESHTOOLS_EXPORT const std::string& name() const;

    MESHTOOLS_EXPORT AttributeRole role() const;

    MESHTOOLS_EXPORT AttributeBinding binding() const;

    MESHTOOLS_EXPORT AttributeComponentType componentType() const;

    MESHTOOLS_EXPORT int elementSize() const;

    MESHTOOLS_EXPORT size_t numElements() const;

    MESHTOOLS_EXPORT const float* floatData() const;

    MESHTOOLS_EXPORT const int* intData() const;

    /**
     * @brief Changes the binding of the attribute. Recomputes the array of values accoring to the new binding.
     * This potentially changes the number of elements in the attribute. Note that data is lost
     * when changing the binding from FACE_VARYING to VERTEX_VARYING or from VERTX_VARYING to PER_FACE.
     * @param binding The new binding
     * @param faceSizes The sizes of the faces of the associated mesh
     * @param indices The face indices of the associated mesh
     * @return True if the mappping is supported
     */
    MESHTOOLS_EXPORT bool changeBinding(AttributeBinding binding,
                                        const std::vector<int>& faceSizes,
                                        const std::vector<int>& indices);

    MESHTOOLS_EXPORT bool isModified() const;
    MESHTOOLS_EXPORT void clearModified();

    /*! \cond PRIVATE */
protected:
    class AttributeImpl;
    MESHTOOLS_EXPORT Attribute(const AttributeImpl&);
    std::unique_ptr<AttributeImpl> m_impl;
    /*! \endcond */
};

/**
 * @brief Supported types of value attributes.
 */
enum class ValueAttributeType
{
    NONE,
    BOOL,
    INT,
    FLOAT,
    VEC2,
    VEC3,
    VEC4,
    QUAT,
    STRING,
};

/**
 * @brief Represents an attribute of a mesh containing a single value.
 */
class ValueAttribute
{
public:
    MESHTOOLS_EXPORT ValueAttribute(const std::string& name, bool value);
    MESHTOOLS_EXPORT ValueAttribute(const std::string& name, int value);
    MESHTOOLS_EXPORT ValueAttribute(const std::string& name, float value);
    MESHTOOLS_EXPORT ValueAttribute(const std::string& name, const Vec2& value);
    MESHTOOLS_EXPORT ValueAttribute(const std::string& name, const Vec3& value);
    MESHTOOLS_EXPORT ValueAttribute(const std::string& name, const Vec4& value);
    MESHTOOLS_EXPORT ValueAttribute(const std::string& name, const Quat& quat);
    MESHTOOLS_EXPORT ValueAttribute(const std::string& name, const std::string& value);

    MESHTOOLS_EXPORT ValueAttribute(const ValueAttribute& other);

    MESHTOOLS_EXPORT ~ValueAttribute();

    MESHTOOLS_EXPORT ValueAttributeType type() const;
    MESHTOOLS_EXPORT const std::string& name() const;

    MESHTOOLS_EXPORT bool get(bool& value) const;
    MESHTOOLS_EXPORT bool get(int& value) const;
    MESHTOOLS_EXPORT bool get(float& value) const;
    MESHTOOLS_EXPORT bool get(Vec2& value) const;
    MESHTOOLS_EXPORT bool get(Vec3& value) const;
    MESHTOOLS_EXPORT bool get(Vec4& value) const;
    MESHTOOLS_EXPORT bool get(Quat& value) const;
    MESHTOOLS_EXPORT bool get(std::string& value) const;

    /*! \cond PRIVATE */
protected:
    class ValueAttributeImpl;
    MESHTOOLS_EXPORT ValueAttribute(const ValueAttributeImpl&);
    std::unique_ptr<ValueAttributeImpl> m_impl;
    /*! \endcond */
};


} // namespace MeshTools
