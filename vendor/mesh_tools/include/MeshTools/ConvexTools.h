// SPDX-FileCopyrightText: Copyright (c) 2024-2025 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
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

/* ------------------------------------------------------------------------------------*/

/**
 * @brief Holds the data of a convex shape.
 *
 */
struct Convex
{
    Convex() = default;
    Convex(const std::vector<Vec3>& vertices, const std::vector<int>& indices, const std::vector<int>& faceSizes)
        : vertices(vertices), indices(indices), faceSizes(faceSizes)
    {
    }
    void clear()
    {
        vertices.clear();
        indices.clear();
        faceSizes.clear();
        neighborFaces.clear();
        neighborEdges.clear();
        facePlanes.clear();
    }

    /**
     * @brief Computes the face planes of a convex shape. This is required for some operations like cutting.
     */
    MESHTOOLS_EXPORT void computeFacePlanes();

    /**
     * @brief Finds the neighbors of the faces in a convex shape. This is information is required for some operations
     * like cutting.
     */
    MESHTOOLS_EXPORT void findFaceNeighbors();

    /**
     * @brief Tests if a convex shape is valid.
     * @return 0 if the convex is valid, 1 if the neighbor information is present but corrupted, 2 if it has invalid
     * topology
     */
    MESHTOOLS_EXPORT int isValid() const;

    /**
     * @brief Tests if a convex shape is convex. The face planes and neighbor information need to be computed before
     * calling this function.
     * @return True if the convex shape is convex, false otherwise
     */
    MESHTOOLS_EXPORT bool isConvex() const;

    /**
     * @brief Computes the volume of a convex shape.
     * @param convex The convex shape to compute the volume for
     * @return The volume of the convex shape, or 0.0f if the shape is invalid
     */
    MESHTOOLS_EXPORT float computeVolume() const;

    /**
     * @brief Tests if a point is inside the convex shape. The face planes need to be computed before calling this
     * function.
     * @param point The point to test
     * @return True if the point is inside the convex shape, false otherwise
     */
    MESHTOOLS_EXPORT bool contains(const Vec3& point) const;

    std::vector<Vec3> vertices; ///< The vertices of the convex shape
    std::vector<int> indices; ///< The indices of the vertices that form the convex shape
    std::vector<int> faceSizes; ///< The sizes of the faces in the convex shape
    std::vector<int> neighborFaces; ///< The neighbors faces of the faces
    std::vector<int> neighborEdges; ///< The global edge of the neighbor face that is adjacent to the face
    std::vector<Plane> facePlanes; ///< The plames of the faces in the convex shape
};

/* ------------------------------------------------------------------------------------*/

/**
 * @brief Computes the convex hull of a set of 2d or 3d points.
 *
 */
class ConvexTools
{
public:
    /**
     * @brief Creates a new ConvexTools object.
     */
    MESHTOOLS_EXPORT ConvexTools();
    MESHTOOLS_EXPORT ~ConvexTools();

    /**
     * @brief Computes the convex hull of a set of 3d points using the monotone chain algorithm.
     * @param points The input points.
     * @param hullIndices The indices of the points which form the convex hull.
     * @return True if the convex hull was successfully computed.
     */

    MESHTOOLS_EXPORT bool compute2dHull(const StaticBuffer<Vec2>& points, std::vector<int>& hullIndices);

    /**
     * @brief Computes the convex hull of a set of 3d points using the monotone chain algorithm.
     * @param points The input points.
     * @param projectionNormal The normal of the plane to project the points onto.
     * @param hullIndices The indices of the points which form the convex hull.
     * @return True if the convex hull was successfully computed.
     */

    MESHTOOLS_EXPORT bool compute2dHull(const StaticBuffer<Vec3>& points,
                                        const Vec3& projectionNormal,
                                        std::vector<int>& hullIndices);

    /**
     * @brief Computes the optimal oriented bounding box of a set of 2d points.
     * It has the minimal area of all possible oriented bounding boxes.
     * @param points The input points.
     * @param center The computed center of the OBB.
     * @param halfExtents The computedhalf extents of the OBB.
     * @param axis The computed first axis of the OBB. The second axis is perpendicular to this axis.
     * @return True if the OBB was successfully computed.
     */
    MESHTOOLS_EXPORT bool computeMinimal2dOBB(const StaticBuffer<Vec2>& points, Vec2& center, Vec2& halfExtents, Vec2& axis);

    /**
     * @brief Computes an oriented bounding box of a set of 3d points. The OBB is not necessarily the optimal one.
     * It is computed by using PCA on the input points and subsequent optimizing rotations around the principal axes.
     * @param numIterations The number of optimization iterations.
     * @param points The input points.
     * @param initialFrame The initial frame of the OBB to start the optimization with.
     * If nullptr, the frame is computed from the input points.
     * @return The computed OBB.
     */
    MESHTOOLS_EXPORT OBB computeOBB(const StaticBuffer<Vec3>& points,
                                    int numIterations = 0,
                                    const Mat33* initialFrame = nullptr);

    /**
     * @brief Computes the axis aligned bounding box (AABB) of a set of 3d points.
     * @param points The input points.
     * @param AABB The resulting axis aligned bounding box as a convex.
     * @return True if the AABB was successfully computed, false otherwise.
     */
    MESHTOOLS_EXPORT bool createAABB(const StaticBuffer<Vec3>& points, Convex& AABB);

    /**
     * @brief Computes the convex hull of a set of 3d points. Not implemented yet.
     * @param points The input points.
     * @param convex The resulting convex hull.
     * @param maxVertices If greater >= 3 the convex hull numVertices points is computed. These points are selected
     * such that that the convex hull is as big as possible. The resulting convex hull is not guaranteed to contain all
     * points
     * @return True if the convex hull was successfully computed.
     */
    MESHTOOLS_EXPORT bool computeHull(const StaticBuffer<Vec3>& points, Convex& convex, int maxVertices = -1);

    /**
     * @brief Cuts a convex shape with a plane. This operation does not require neighbor information.
     * @param convex The convex shape to cut
     * @param plane The plane to cut the convex shape with
     * @param result The resulting convex shape after cutting. The result is the part of the convex shape that lies
     * below the plane such that the normal of the cut face equals the plane normal.
     * @return True if the cut was successful, false otherwise. T
     */
    MESHTOOLS_EXPORT bool cutConvex(const Convex& convex, const Plane& plane, Convex& result);

    /**
     * @brief Cuts two convex shapes and returns the resulting convex shape. This operation needs the face planes of
     * convex1.
     * @param convex1 The first convex shape to cut
     * @param convex2 The second convex shape to cut
     * @param result The resulting convex shape after cutting. The result is the part of the first convex shape that
     * lies inside the second convex shape.
     * @return True if the cut was successful, false otherwise, e.g. if neighbor information is missing or the convexes
     * are not valid.
     */
    MESHTOOLS_EXPORT bool cutConvexes(const Convex& convex1, const Convex& convex2, Convex& result);

    /**
     * @brief Computes an approximate convex hull of a set of points using the supporting planes method.
     * @param points The points to compute the convex hull for
     * @param convex The resulting convex hull
     * @param level
     * level 0: OBB, 6 faces
     * level 1: corners chopped off, 14 faces
     * level 2: icosahedron, 20 faces
     * level 3: corners and edges chopped off, 26 faces
     * level 4: subdivide icosahedron, 42 faces
     * level 5: optimized faces (slower)
     * @param maxFaces The maximum number of faces in the resulting convex hull. This parameter is only used if the
     * level is 5.
     * @return True if the convex hull was computed successfully, false otherwise
     */
    MESHTOOLS_EXPORT bool computeApproximateConvexHull(const StaticBuffer<Vec3>& points,
                                                       Convex& convex,
                                                       int level = 0,
                                                       int maxFaces = 0);

    /**
     * @brief Creates an approximate convex decomposition of a set of vertices.
     * @param vertices The vertices of the input mesh.
     * @param indices The indices of the input mesh.
     * @param faceSizes The sizes of the faces in the input mesh.
     * @param convexes The resulting convex shapes.
     * @param level The level of detail of the convexes. See computeApproximateConvexHull for details.
     * @param maxFaces The level of detail of the convexes. See computeApproximateConvexHull for details.
     * @return True if the decomposition was successful, false otherwise.
     */
    MESHTOOLS_EXPORT bool createAppriximateConvexDecomposition(const StaticBuffer<Vec3>& vertices,
                                                               StaticBuffer<int>& indices,
                                                               StaticBuffer<int>& faceSizes,
                                                               std::vector<Convex>& convexes,
                                                               int level = 0,
                                                               int maxFaces = 0);

    /*! \cond PRIVATE */
protected:
    class ConvexToolsImpl;
    MESHTOOLS_EXPORT ConvexTools(const ConvexToolsImpl&);
    std::unique_ptr<ConvexToolsImpl> m_impl;
    /*! \endcond */
};

}
