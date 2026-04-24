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

#include "Attribute.h"
#include "Defines.h"
#include "Vec.h"

#include <memory>
#include <string>
#include <vector>

namespace MeshTools
{


/**
 * @brief Represents a polygonal mesh.
 *
 */
class Mesh
{
public:
    /// Constructs an empty Mesh
    MESHTOOLS_EXPORT Mesh();
    MESHTOOLS_EXPORT ~Mesh();

    MESHTOOLS_EXPORT void cloneFrom(const Mesh& other);

    /**
     * @brief Clear the contents of the mesh.
     */
    MESHTOOLS_EXPORT void clear();

    MESHTOOLS_EXPORT bool isModified() const;
    MESHTOOLS_EXPORT void clearModified();

    MESHTOOLS_EXPORT void setVertices(size_t numVertices, const Vec3* vertices);
    MESHTOOLS_EXPORT void setIndices(size_t numIndices, const int* indices);
    MESHTOOLS_EXPORT void setFaceSizes(size_t numFaces, const int* faceSizes);

    MESHTOOLS_EXPORT void setVertices(const std::vector<Vec3>& vertices);
    MESHTOOLS_EXPORT void setIndices(const std::vector<int>& indices);
    MESHTOOLS_EXPORT void setFaceSizes(const std::vector<int>& faceSizes);
    MESHTOOLS_EXPORT void setColor(const Vec4& color);

    MESHTOOLS_EXPORT void clearAttributes();
    MESHTOOLS_EXPORT void clearValueAttributes();
    MESHTOOLS_EXPORT void addAttribute(std::shared_ptr<Attribute> attribute);
    MESHTOOLS_EXPORT void addValueAttribute(std::shared_ptr<ValueAttribute> attribute);

    MESHTOOLS_EXPORT const std::vector<Vec3>& vertices() const;
    MESHTOOLS_EXPORT const std::vector<int>& indices() const;
    MESHTOOLS_EXPORT const std::vector<int>& faceSizes() const;
    MESHTOOLS_EXPORT const std::vector<std::shared_ptr<Attribute>>& attributes() const;
    MESHTOOLS_EXPORT const std::vector<std::shared_ptr<ValueAttribute>>& valueAttributes() const;
    MESHTOOLS_EXPORT Vec4 color() const;

    /**
     * @brief Returns the number of the prototype mesh in the stage.
     * When >= 0, the geometry of this mesh is ignored and the geometry from stage.prototypes[prototypeNr] is used.
     */
    MESHTOOLS_EXPORT int getPrototypeNr() const;

    /**
     * @brief Returns the prototype mesh if this mesh is an instance, or nullptr if it's a regular mesh.
     */
    MESHTOOLS_EXPORT std::shared_ptr<Mesh> getPrototype() const;

    /**
     * @brief Sets the prototype number for this mesh instance.
     * Set to -1 for a regular mesh (uses its own geometry).
     * Set to >= 0 to make this an instance that references stage.prototypes[prototypeNr].
     */
    MESHTOOLS_EXPORT void setPrototypeNr(int prototypeNr, std::shared_ptr<Mesh> prototype);

    /**
     * @brief Sets the transform (global pose) of the mesh.
     * @param transform The transformation to be stores (not applied to the vertices of the mesh)
     */
    MESHTOOLS_EXPORT void setTransform(const AffineTransform& transform);

    /**
     * @brief Returns the transform of the mesh.
     */
    MESHTOOLS_EXPORT const AffineTransform& transform() const;

    /**
     * @brief Triangulates the faces if they have changed since the last call.
     * @return The triangle indices
     */
    MESHTOOLS_EXPORT const std::vector<int>& getTriIndices() const;

    /**
     * @brief Returns for each triangle the face it was created from.
     */
    MESHTOOLS_EXPORT const std::vector<size_t>& getTriParentFaces() const;

    /**
     * @brief A buffer of indices used to map face varying attributes
     * to the triangulated mesh.
     */
    MESHTOOLS_EXPORT const std::vector<size_t>& getTriParentAttributeLocations() const;

    /**
     * @brief Computes an island number for each face if the mesh has changed since the last call.
     * @param useWeldedVertices If true, vertices are considered iqual if they are are co-located
     * An island is an isolated connected group of connected faces and vertices.
     */
    MESHTOOLS_EXPORT const std::vector<int>& getFaceIslands(bool useWeldedVertices = true) const;

    /**
     * @brief Identifies the vertices that are enclosed by closed sub-surfaces.
     * @param faceEnclosed A vector of booleans indicating if a vertex is enclosed or not
     */
    MESHTOOLS_EXPORT void findEnclosedVertices(std::vector<bool>& vertexEnclosed) const;
    /**
     * @brief Removes islands from the mesh.
     * An island is an isolated connected group of connected faces and vertices.
     * @param maximalFaceCount The maximal number of faces an island may have to be removed
     * @param enclosedIslands If true, enclosed islands of any size are removed
     */
    MESHTOOLS_EXPORT void removeFaceIslands(int maximalFaceCount, bool enclosedIslands, bool useWeldedVertices = true);

    /**
     * @brief Sets the name of the mesh.
     */
    MESHTOOLS_EXPORT void setName(const std::string& name);

    /**
     * @brief Returns the name of the mesh.
     */
    MESHTOOLS_EXPORT const std::string& name() const;

    /**
     * @brief Sets the path of the mesh as used in a USD file.
     */
    MESHTOOLS_EXPORT void setPath(const std::string& path);

    /**
     * @brief Returns the path of the mesh as used in a USD file.
     */
    MESHTOOLS_EXPORT const std::string& path() const;

    /**
     * @brief Sets the duplication set of the mesh.
     *     The set number is used to identify meshes that are duplicates of each other.
     */
    MESHTOOLS_EXPORT void setDuplicationSet(int set);

    /**
     * @brief Gets the duplication set of the mesh.
     * Meshes with the same set number are duplicates of each other (possibly using fuzzy matching)
     * If the number is -1 the mesh is not part of a duplication set.
     * The value is set by the DuplicationChecker.
     */
    MESHTOOLS_EXPORT int getDuplicationSet() const;

    /**
     * @brief Defines whether the mesh is visible or not.
     */
    MESHTOOLS_EXPORT void setVisible(bool visible);

    /**
     * @brief Returns the visibility of the mesh.
     */
    MESHTOOLS_EXPORT bool isVisible() const;

    /**
     * @brief Defines whether a face is visible or not.
     */
    MESHTOOLS_EXPORT void setFaceVisible(int faceNr, float visible);

    /**
     * @brief Returns the visibility of all faces.
     */
    MESHTOOLS_EXPORT const std::vector<float>& getFaceVisible() const;

    /**
     * @brief Returns the axis-aligned bounding box of the mesh, not considering the mesh transformation.
     */
    MESHTOOLS_EXPORT const MeshTools::Bounds3& getAABB() const;

    /**
     * @brief Returns the orientation-aligned bounding box of the mesh in the local coordinate system.
     * The main axes are computed using principal component analysis.
     * The extents correspond to the extents of the bounding box in the local coordinate system.
     * @param optimize If true, optimzes towards the volume minimizing OBB using multiple 2d projections. This increases
     * the computation time. The OBB is oriented such that the center of mass of the surface has negative coordinates in
     * the local coordinate system. The result is only computed once and cached for subsequent calls until the mesh
     * changes.
     */
    MESHTOOLS_EXPORT const MeshTools::OBB& getOBB(bool optimize = false) const;

    /**
     * @brief Returns the volume of the mesh.
     * The volume is computed by summing the signed volumes of the tetrahedra
     * formed by the origin and the triangles of the mesh.
     * This method yields the exact volume for closed manifold meshes.
     * The value is only computed once and cached for subsequent calls until the mesh changes.
     */
    MESHTOOLS_EXPORT float getVolume() const;

    /**
     * @brief Returns the surface area of the mesh.
     * The surface area is computed by summing the areas of the faces of the mesh.
     * The value is only computed once and cached for subsequent calls until the mesh changes.
     */
    MESHTOOLS_EXPORT float getSurfaceArea() const;

    /**
     * @brief Clusters islands by merging islands that are smaller than the given size in the local coordinate system.
     * The result is stored in the island number of each face.
     * The result is only computed once and cached for subsequent calls until the mesh changes.
     * @param clusterSize The desired size of the clusters
     */
    MESHTOOLS_EXPORT void clusterIslands(float clusterSize);

    /**
     * @brief Returns the vertices of the mesh transformed by the mesh transform.
     * The transformed vertices are computed once and cached for subsequent calls until the mesh or its transform
     * changes.
     */
    MESHTOOLS_EXPORT const std::vector<Vec3>& getTransformedVertices() const;

    /**
     * @brief Find the closest face of the mesh that is intersected by the given ray.
     * The intersection is computed in the local coordinate system of the mesh.
     * For a global intersection test, the ray has to be transformed to the local coordinate system of the mesh first.
     * @param ray The ray to intersect with the mesh
     * @param t The distance along the ray to the intersection point
     * @param faceNr The number of the face that was intersected
     * @param normal The normal of the mesh at the hit location
     * @param clip (Optional) A clip plane to clip the mesh against
     * @return Boolean indicating if an intersection was found
     */
    MESHTOOLS_EXPORT bool raycast(const Ray& ray, float& t, size_t& faceNr, Vec3& normal, const Vec3* clip = nullptr) const;

    /**
     * @brief Transforms the vertices of the mesh.
     * @param transform The transformation to apply
     */
    MESHTOOLS_EXPORT void transformVertices(const AffineTransform& transform);

    /*! \cond PRIVATE */
protected:
    class MeshImpl;
    MESHTOOLS_EXPORT Mesh(const MeshImpl&);
    std::unique_ptr<MeshImpl> m_impl;
    /*! \endcond */
};

/**
 * @brief Finds the neighbor triangles of each triangle in a mesh.
 * @param triIndices The triangle indices. Negative or equal indices are interpreted as deleted triangles.
 * @param neighbors An integer array of the size of three times the number of triangles.
 * The entry at index 3 times i + j contains the neighbor triangle i across edge j.
 * @param trianglesOnly If true, the neighbors vector contains the triangle number of the neighbor triangle.
 * If false, an entry in the neighbors vector contains the three times the number of the neighbor triangle plus the
 * local edge number that points back to the original triangle.
 * @return True if the mesh is manifold, false otherwise.
 */
MESHTOOLS_EXPORT bool findTriangleNeighbors(const StaticBuffer<int>& triIndices,
                                            std::vector<int>& neighbors,
                                            bool trianglesOnly = true);


struct MeshFaceNeighbor
{
    int faceNr = -1; ///< The adjacet face number.
    int faceFirstIndex = -1; ///< The first index of the neighbor face in the indices array.
    int localEdgeNr = 0; ///< The local edge number in the neighbor face that points back to the original face.
};

/**
 * @brief Finds the neighbor faces of each face in a mesh.
 * @param indices The vertex indices of the faces
 * @param faceSizes The number of vertices of each face
 * @param neighbors An array of MeshFaceNeighbor structs.
 * The entry at index (firstFaceIndex + localEdgeNr) contains the neighbor of faceNr across edge localEdgeNr.
 * The neighbor struct contains the number of the neighbor face and the number of the edge in the neighbor face which
 * points back to the original face.
 * @return True if the mesh is manifold, false otherwise.
 */

MESHTOOLS_EXPORT bool findPolygonNeighbors(const StaticBuffer<int>& indices,
                                           const StaticBuffer<int>& faceSizes,
                                           std::vector<MeshFaceNeighbor>& neighbors);

/**
 * @brief Creates a new triangle mesh with sharp edges split
 * @param vertices The vertices of the input triangle mesh
 * @param triIndices The triangle indices of the input triangle mesh
 * @param featureMaxDot The maximum dot product of the normals of two adjacent faces to consider an edge as sharp
 * @param splitTriIds The triangle indices of the output triangle mesh
 * @param vertexMap Contains for each output vertex the index of the corresponding input vertex to map vertex positions,
 * normals, etc.
 *
 */
MESHTOOLS_EXPORT void splitSharpEdges(const StaticBuffer<MeshTools::Vec3>& vertices,
                                      const StaticBuffer<int>& triIndices,
                                      float featureMaxDot,
                                      std::vector<int>& splitTriIds,
                                      std::vector<int>& vertexMap);

} // namespace MeshTools
