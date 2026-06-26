// SPDX-FileCopyrightText: Copyright (c) 2024-2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
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

#include <vector>

namespace MeshTools
{

/**
 * @brief Compute the point intersection of a ray with a triangle
 * @param ray The ray to intersect
 * @param p0 The first vertex of the triangle
 * @param p1 The second vertex of the triangle
 * @param p2 The third vertex of the triangle
 * @param t The distance along the ray to the intersection point
 * @param u The first barycentric coordinate of the intersection point. The intersection point is p0 + u * (p1 - p0) + v
 * * (p2 - p0).
 * @param v The second barycentric coordinate of the intersection point. The intersection point is p0 + u * (p1 - p0) +
 * v * (p2 - p0).
 * @return True if the ray intersects the triangle, false otherwise
 */
MESHTOOLS_EXPORT bool rayTriangleIntersection(
    const Ray& ray, const Vec3& p0, const Vec3& p1, const Vec3& p2, float& t, float& u, float& v);

/**
 * @brief Compute the entry and exit points of a ray with an axis-aligned bounding box
 * @param ray The ray to intersect
 * @param bounds The axis-aligned bounding box to intersect
 * @param t0 The distance along the ray to the entry point
 * @param t1 The distance along the ray to the exit point
 * @return True if the ray intersects the bounding box, false otherwise
 */
MESHTOOLS_EXPORT bool rayBoundsIntersection(const Ray& ray, const Bounds3& bounds, float& t0, float& t1);

/**
 * @brief Compute the intersection of a ray with a sphere
 * @param ray The ray to intersect
 * @param p The center of the sphere
 * @param r The radius of the sphere
 * @param t The distance along the ray to the closer intersection point
 * @return True if the ray intersects the sphere, false otherwise
 */
MESHTOOLS_EXPORT bool raySphereIntersection(const Ray& ray, const Vec3& p, float r, float& t);

/**
 * @brief Compute the intersection of a ray with a capsule
 * @param ray The ray to intersect
 * @param p0 The start of the capsule axis
 * @param p1 The end of the capsule axis
 * @param r The radius of the capsule
 * @param t The distance along the ray to the closer intersection point
 * @return True if the ray intersects the capsule, false otherwise
 */
MESHTOOLS_EXPORT bool rayCapsuleIntersection(const Ray& ray, const Vec3& p0, const Vec3& p1, float r, float& t);

/**
 * @brief Computes the closest point on a ray to a point p
 * @param p The query point
 * @param ray The ray
 * @return The closest point on the ray to p as ray origin + t * ray direction
 */
MESHTOOLS_EXPORT float getClosestPointOnLine(const Vec3& p, const Ray& ray);

/**
 * @brief Computes the closest point on a segment to the point p
 * @param p The query point
 * @param p0 The start of the segment
 * @param p1 The end of the segment
 * @param inside If not nullptr, set to true if the closest point is inside the segment, false otherwise
 * @param t If not nullptr, set to the distance along the segment to the closest point
 * @return The closest point on the segment to p
 */
Vec3 getClosestPointOnSegment(const Vec3& p, const Vec3& p0, const Vec3& p1, bool* inside = nullptr, float* t = nullptr);

/**
 * @brief Computes the closest point on a triangle to the point p
 * @param p The query point
 * @param p0 The first vertex of the triangle
 * @param p1 The second vertex of the triangle
 * @param p2 The third vertex of the triangle
 * @param inside If not nullptr, set to true if the closest point is inside the triangle, false otherwise
 * @return The barycentric coordinates b of the closest point on the triangle to p
 * The closest point is at location b.x * p0 + b.y * p1 + b.z * p2
 */
MESHTOOLS_EXPORT Vec3
getClosestPointOnTriangle(const Vec3& p, const Vec3& p0, const Vec3& p1, const Vec3& p2, bool* inside = nullptr);

/**
 * @brief Computes the closest point on a triangle to the point p.
 * This version uses double precision internally and is more accurate but slower than the single precision version.
 * This function is useful to handle triangles with bad shapes like slivers.
 * @param p The query point
 * @param p0 The first vertex of the triangle
 * @param p1 The second vertex of the triangle
 * @param p2 The third vertex of the triangle
 * @param inside If not nullptr, set to true if the closest point is inside the triangle, false otherwise
 * @return The barycentric coordinates b of the closest point on the triangle to p
 * The closest point is at location b.x * p0 + b.y * p1 + b.z * p2
 */
MESHTOOLS_EXPORT Vec3
getClosestPointOnTriangleD(const Vec3& p, const Vec3& p0, const Vec3& p1, const Vec3& p2, bool* inside = nullptr);

/**
 * @brief Computes the closest point on an axis-aligned bounding box to the point p
 * @param p The query point
 * @param bounds The axis-aligned bounding box
 * @return The closest point on the bounding box to p
 */
MESHTOOLS_EXPORT Vec3 getClosestPointOnBounds(const Vec3& p, const Bounds3& bounds);

/**
 * @brief Computes the closest points on two rays
 * @param line0 The first line
 * @param line1 The second line
 * @param t0 The distance along the first line to its closest point
 * @param t1 The distance along the second line to its closest point
 * @return False if the lines are parallel, true otherwise
 */
MESHTOOLS_EXPORT bool getClosestPointsOnLines(const Ray& line0, const Ray& line1, float& t0, float& t1);

/**
 * @brief Computes the closest points on a line and a segment
 * @param line The line
 * @param p0 The start of the segment
 * @param p1 The end of the segment
 * @param t0 The distance along the line to its closest point
 * @param t1 The distance along the segment to its closest point
 */
MESHTOOLS_EXPORT void getClosestPointsOnLineAndSegment(
    const Ray& line, const Vec3& p0, const Vec3& p1, float& t0, float& t1);

/**
 * @brief Computes the closest points on two segments
 * @param p0 The start of the first segment
 * @param p1 The end of the first segment
 * @param q0 The start of the second segment
 * @param q1 The end of the second segment
 * @param t0 The distance along the first segment to its closest point
 * @param t1 The distance along the second segment to its closest point
 */
MESHTOOLS_EXPORT void getClosestPointsOnSegments(
    const Vec3& p0, const Vec3& p1, const Vec3& q0, const Vec3& q1, float& t0, float& t1);

/**
 * @brief Compute the distance squared from a point to an axis-aligned bounding box
 * @param p The query point
 * @param bounds The axis-aligned bounding box
 * @return The distance squared from p to the bounding box
 */
MESHTOOLS_EXPORT float distanceToBoundsSquared(const Vec3& p, const Bounds3& bounds);

/**
 * @brief Compute the center of a sphere that passes through 3 points of a triangle
 * @param p0 The first vertex of the triangle
 * @param p1 The second vertex of the triangle
 * @param p2 The third vertex of the triangle
 * @return The center of the sphere that passes through the 3 points of the triangle
 */
MESHTOOLS_EXPORT Vec3 getCircumCenterOfTriangle(const Vec3& p0, const Vec3& p1, const Vec3& p2);

/**
 * @brief Compute the center of a sphere that passes through 4 points of a tetrahedron
 * @param p0 The first vertex of the tetrahedron
 * @param p1 The second vertex of the tetrahedron
 * @param p2 The third vertex of the tetrahedron
 * @param p3 The fourth vertex of the tetrahedron
 * @return The center of the sphere that passes through the 4 points of the tetrahedron
 */
MESHTOOLS_EXPORT Vec3 getCircumCenterOfTetrehedron(const Vec3& p0, const Vec3& p1, const Vec3& p2, const Vec3& p3);

/**
 * @brief Compute the barycentric coordinates of a point with respect to a tetrahedron
 * @param p The query point
 * @param p0 The first vertex of the tetrahedron
 * @param p1 The second vertex of the tetrahedron
 * @param p2 The third vertex of the tetrahedron
 * @param p3 The fourth vertex of the tetrahedron
 * @return The barycentric coordinates of p with respect to the tetrahedron
 */
MESHTOOLS_EXPORT Vec4
getBarycentricCoordinates(const Vec3& p, const Vec3& p0, const Vec3& p1, const Vec3& p2, const Vec3& p3);

/**
 * @brief Compute the barycentric coordinates of a point with respect to a triangle. For the computation, the point is
 * projected into the plane of the triangle.
 * @param p The query point
 * @param p0 The first vertex of the triangle
 * @param p1 The second vertex of the triangle
 * @param p2 The third vertex of the triangle
 * @return The barycentric coordinates of p with respect to the triangle
 */
MESHTOOLS_EXPORT Vec3 getBarycentricCoordinates(const Vec3& p, const Vec3& p0, const Vec3& p1, const Vec3& p2);

/**
 * @brief Compute a point on a spline curve
 * @param p0 The first control point of the spline
 * @param t0 The tangent at the first control point of the spline
 * @param p1 The second control point of the spline
 * @param t1 The tangent at the second control point of the spline
 * @param t The location along the spline (0 <= t <= 1)
 * @return The point on the spline at location t
 */
MESHTOOLS_EXPORT Vec3 interpolateSpline(const Vec3& p0, const Vec3& t0, const Vec3& p1, const Vec3& t1, float t);

/**
 * @brief Compute a point on a curved triangle
 * @param p0 The first vertex of the triangle
 * @param p1 The second vertex of the triangle
 * @param p2 The third vertex of the triangle
 * @param n0 The normal at the first vertex of the triangle
 * @param n1 The normal at the second vertex of the triangle
 * @param n2 The normal at the third vertex of the triangle
 * @param t0 The location along the triangle (0 <= t0 <= 1)
 * @param t1 The location along the triangle (0 <= t1 <= 1)
 * @return The point on the curved triangle at location t0, t1
 */
MESHTOOLS_EXPORT Vec3 interpolateCurvedTriangle(
    const Vec3& p0, const Vec3& p1, const Vec3& p2, const Vec3& n0, const Vec3& n1, const Vec3& n2, float t0, float t1);

/**
 * @brief Computes the angle weighted normals of a triangle mesh
 * @param vertices The vertices of the triangle mesh
 * @param triIds The triangle indices of the triangle mesh
 * @param normals The computed angle weighted normals of the triangle mesh
 */
MESHTOOLS_EXPORT void computeAngleWeightedNormals(const StaticBuffer<Vec3>& vertices,
                                                  const StaticBuffer<int>& triIds,
                                                  std::vector<Vec3>& normals);


/**
 * @brief Computes the angle weighted normals of a polygonal mesh
 * @param vertices The vertices of the triangle mesh
 * @param indices The face indices of the polygonal mesh
 * @param faceSizes The face sizes of the polygonal mesh.
 * @param normals The computed angle weighted normals of the triangle mesh
 */
MESHTOOLS_EXPORT void computeAngleWeightedNormals(const StaticBuffer<Vec3>& vertices,
                                                  const StaticBuffer<int>& indices,
                                                  const StaticBuffer<int>& faceSizes,
                                                  std::vector<Vec3>& normals);

/**
 * @brief Smoothes the normals of a triangle mesh
 * @param triIds The triangle indices of the triangle mesh
 * @param normals The normals of the triangle mesh
 * @param numIterations The number of iterations for the smoothing
 */
MESHTOOLS_EXPORT void smoothNormals(const StaticBuffer<int>& triIds, std::vector<Vec3>& normals, int numIterations);

/**
 * @brief Tests whether a triangle intersects an axis-aligned bounding box
 * @param p0 The first vertex of the triangle
 * @param p1 The second vertex of the triangle
 * @param p2 The third vertex of the triangle
 * @param center The center of the axis-aligned bounding box
 * @param halfExtents The half extents of the axis-aligned bounding box
 * @return True if the triangle intersects the axis-aligned bounding box, false otherwise
 */
MESHTOOLS_EXPORT bool testBoxTriangleIntersection(
    const Vec3& p0, const Vec3& p1, const Vec3& p2, const Vec3& center, const Vec3& halfExtents);


/** * @brief Tests whether a triangle intersects a square
 * @param p0 The first vertex of the triangle
 * @param p1 The second vertex of the triangle
 * @param p2 The third vertex of the triangle
 * @param center The center of the square
 * @param halfExtents The half extents of the square
 * @return True if the triangle intersects the square, false otherwise
 */
MESHTOOLS_EXPORT bool testSquareTriangleIntersection(
    const Vec2& p0, const Vec2& p1, const Vec2& p2, const Vec2& center, const Vec2& halfExtents);

/**
 * @brief Computes the object-aligned bounding of a set of points
 * @param vertices The vertices of the point cloud
 * @param optimize If true, optimzes towards the volume minimizing OBB using multiple 2d projections. This increases the
 * computation time.
 * @param sign If not nullptr, set to -1 or 1 dependent on the location of the center of mass relatie to the OBB.
 * This value can be used to distinguish between mirrored shapes. Mirrored shapes have the same OBB but a different
 * sign.
 * @return The object-aligned bounding box of the point cloud.
 * The OBB is oriented such that the center of mass has negative coordinates in the object-aligned frame.
 */
MESHTOOLS_EXPORT OBB computeOBBFromPoints(const StaticBuffer<Vec3>& vertices, bool optimize = false, int* sign = nullptr);

/**
 * @brief Computes the object-aligned bounding box of a polygonal mesh
 * @param vertices The vertices of the polygonal mesh
 * @param faceIds The face indices of the polygonal mesh
 * @param faceSizes The face sizes of the polygonal mesh
 * @param optimize If true, optimzes towards the volume minimizing OBB using multiple 2d projections. This increases the
 * computation time.
 * @param sign If not nullptr, set to -1 or 1 dependent on the location of the center of mass relatie to the OBB.
 * This value can be used to distinguish between mirrored shapes. Mirrored shapes have the same OBB but a different
 * sign.
 * @param surfaceArea If not nullptr, set to the surface area of the polygonal mesh
 * @return The object-aligned bounding box of the polygonal mesh.
 * The OBB is oriented such that the center of mass has negative coordinates in the object-aligned frame.
 */
MESHTOOLS_EXPORT OBB computeOBBFromSurface(const StaticBuffer<Vec3>& vertices,
                                           const StaticBuffer<int>& faceIds,
                                           const StaticBuffer<int>& faceSizes,
                                           bool optimize = false,
                                           int* sign = nullptr,
                                           float* surfaceArea = nullptr);

/**
 * @brief Computes the volume enclosed by a polygonal mesh
 * The volume is computed by summing the signed volumes of the tetrahedra
 * formed by the origin and the triangles of the mesh.
 * This method yields the exact volume for closed manifold meshes.
 * @param vertices The vertices of the polygonal mesh
 * @param faceIds The face indices of the polygonal mesh
 * @param faceSizes The face sizes of the polygonal mesh. If the vector is empty, faces are assumed to be triangles.
 * @return The volume enclosed by the polygonal mesh
 */
MESHTOOLS_EXPORT float computeVolume(const StaticBuffer<Vec3>& vertices,
                                     const StaticBuffer<int>& faceIds,
                                     const StaticBuffer<int>& faceSizes);

/**
 * @brief Computes the surface area of a polygonal mesh
 * @param vertices The vertices of the polygonal mesh
 * @param faceIds The face indices of the polygonal mesh
 * @param faceSizes The face sizes of the polygonal mesh. If the vector is empty, faces are assumed to be triangles.
 * @return The surface area of the polygonal mesh
 */
MESHTOOLS_EXPORT float computeSurfaceArea(const StaticBuffer<Vec3>& vertices,
                                          const StaticBuffer<int>& faceIds,
                                          const StaticBuffer<int>& faceSizes);

/**
 * @brief Computes the simulation properties of a rigid body defined by a polygonal mesh
 * @param vertices The vertices of the polygonal mesh
 * @param faceIds The face indices of the polygonal mesh
 * @param faceSizes The face sizes of the polygonal mesh
 * @param density The density of the rigid body
 * @param pose The pose of the rigid body in which the inertia tensor is diagonal
 * @param mass The mass of the rigid body
 * @param inertia The inertia tensor of the rigid body in the object-aligned frame
 * @param halfExtents The half extents of the object-aligned bounding box of the rigid body
 */
MESHTOOLS_EXPORT void computeSimulationProperties(const StaticBuffer<Vec3>& vertices,
                                                  const StaticBuffer<int>& faceIds,
                                                  const StaticBuffer<int>& faceSizes,
                                                  float density,
                                                  Transform& pose,
                                                  float& mass,
                                                  Vec3& inertia,
                                                  Vec3& halfExtents);

/**
 * @brief Computes the simulation properties of a capsule,
 * The capsule is assumed to be aligned with the x-axis
 * @param capsuleRadius The radius of the capsule
 * @param capsuleLength The height of the capsule
 * @param density The density of the capsule
 * @param mass The mass of the capsule
 * @param inertia The diagonal inertia tensor of the capsule
 */

MESHTOOLS_EXPORT void computeSimulationProperties(
    float capsuleRadius, float capsuleLength, float density, float& mass, Vec3& inertia);

/**
 *@brief Samples the volume enclosed by a polygonal mesh densly with particles.
 * @param vertices The vertices of the polygonal mesh
 * @param faceIds The face indices of the polygonal mesh
 * @param faceSizes The face sizes of the polygonal mesh
 * @params radius The radius of the particles
 */
MESHTOOLS_EXPORT void createParticlePositions(const StaticBuffer<Vec3>& vertices,
                                              const StaticBuffer<int>& faceIds,
                                              const StaticBuffer<int>& faceSizes,
                                              float radius,
                                              std::vector<Vec3>& particlePositions);

/**
 *@brief Computes the curvature perpendicualar to an edge shared by two triangles.
 * The curvature is computed as the inverse of the circumradius of the projections of the points e0, p0 and p1
 * along the edge direction.
 * @param e0 The first point defining the edge
 * @param e1 The second point defining the edge
 * @param p0 The point of the first triangle opposite to the edge
 * @param p1 The point of the second triangle opposite to the edge
 * @return The curvature perpendicular to the edge
 */
MESHTOOLS_EXPORT float getEdgeCurvature(const Vec3& e0, const Vec3& e1, const Vec3& p0, const Vec3& p1);

/**
 * @brief Computes the curvature of a line defined by three consecutive points
 * @param p0 The first point of the line
 * @param p1 The second point of the line
 * @param p2 The third point of the line
 * @return The curvature of the line at the second point
 */
MESHTOOLS_EXPORT float getLineCurvature(const Vec3& p0, const Vec3& p1, const Vec3& p2);

/**
 * @brief Create a Box mesh centered at the origin
 * @param halfExtents The half extents of the box (half width, half height, half depth)
 * @param vertices The vertices of the box
 * @param faceIds The face indices of the box
 * @param faceSizes The face sizes of the box
 */
MESHTOOLS_EXPORT void createBox(const Vec3& halfExtents,
                                std::vector<Vec3>& vertices,
                                std::vector<int>& faceIds,
                                std::vector<int>& faceSizes);

/**
 * @brief Create a Box mesh centered at the origin with sharp edges
 * @param halfExtents The half extents of the box (half width, half height, half depth)
 * @param vertices The vertices of the box
 * @param normals The normals at the vertex positions
 * @param faceIds The face indices of the box
 * @param faceSizes The face sizes of the box
 */
MESHTOOLS_EXPORT void createSharpBox(const Vec3& halfExtents,
                                     std::vector<Vec3>& vertices,
                                     std::vector<Vec3>& normals,
                                     std::vector<int>& faceIds,
                                     std::vector<int>& faceSizes);

/**
 * @brief Create a Capsule mesh centered at the origin and extending along the x-axis
 * @param radius The radius of the capsule
 * @param halfLength The half length of the capsule
 * @param resolution The resolution of the mesh
 * @param vertices The vertices of the capsule
 * @param normals The normals at the vertex positions
 * @param faceIds The face indices of the capsule
 * @param faceSizes The face sizes of the capsule
 */
MESHTOOLS_EXPORT void createCapsule(float radius,
                                    float halfLength,
                                    int resolution,
                                    std::vector<Vec3>& vertices,
                                    std::vector<Vec3>& normals,
                                    std::vector<int>& faceIds,
                                    std::vector<int>& faceSizes);

/**
 * @brief Identify points that are close to each other and create a merge map
 * @param points The points to merge
 * @param idMap The map from the original point indices to the merged point indices
 * Points that are merged have the same index in the idMap
 * @param threshold The distance threshold for merging points
 * @return The number of merged points
 */
MESHTOOLS_EXPORT int mergePoints(std::vector<Vec3>& points, std::vector<int>& idMap, float threshold = 0.0f);

/**
 * @brief Fit a mesh to another mesh without vertex correspondences using an affine transformation
 * @param fromVertices The vertices of the source mesh
 * @param fromFaceIds The face indices of the source mesh
 * @param fromFaceSizes The face sizes of the source mesh
 * @param toVertices The vertices of the target mesh
 * @param toFaceIds The face indices of the target mesh
 * @param toFaceSizes The face sizes of the target mesh
 * @param optimizeOBBs If true, optimzes towards the volume minimizing OBB using multiple 2d projections. This increases
 * the computation time.
 * @param numIterations The number of iterations for the fitting.
 * @param maxError If not null, the function returns the maximal distance between the source and target mesh after
 * fitting.
 * @param connectionLines If not null, the function returns the connection lines between the source vertices and closest
 * points on the target mesh.
 * @return The affine transformation that fits the source mesh to the target mesh
 */
MESHTOOLS_EXPORT AffineTransform fitMeshMeshTransform(const StaticBuffer<Vec3>& fromVertices,
                                                      const StaticBuffer<int>& fromFaceIds,
                                                      const StaticBuffer<int>& fromFaceSizes,
                                                      const StaticBuffer<Vec3>& toVertices,
                                                      const StaticBuffer<int>& toFaceIds,
                                                      const StaticBuffer<int>& toFaceSizes,
                                                      bool optimizeOBBs = false,
                                                      int numIterations = 0,
                                                      float* maxError = nullptr,
                                                      std::vector<Vec3>* connectionLines = nullptr);

/**
 * @brief Fit a point cloud to another point cloud with correspondences using an affine transformation
 * @param sourcePoints The points of the source point cloud
 * @param targetPoints The points of the target point cloud
 * @param weights If privided weights the points in the fitting
 * @return The affine transformation that fits the source point cloud to the target point cloud
 */
MESHTOOLS_EXPORT AffineTransform fitPointTransform(const StaticBuffer<Vec3>& sourcePoints,
                                                   const StaticBuffer<Vec3>& targetPoints,
                                                   const StaticBuffer<float>* weights = nullptr);


/**
 * @brief Compute the intersection of two rays in 2d
 * @param ray0 The first ray
 * @param ray1 The second ray
 * @param t0 The fraction along the first ray to the intersection point
 * @param t1 The fraction along the second ray to the intersection point
 */
MESHTOOLS_EXPORT bool rayRayIntersection2d(const Ray2& ray0, const Ray2& ray1, float& t0, float& t1);

/**
 * @brief Compute the intersection of two rays in 3d when observed from a given direction
 * @param ray0 The first ray
 * @param ray1 The second ray
 * @param projDir The projection direction
 * @param t0 The fraction along the first ray to the intersection point
 * @param t1 The fraction along the second ray to the intersection point
 */
MESHTOOLS_EXPORT bool rayRayIntersection2d(const Ray& ray0, const Ray& ray1, const Vec3& projDir, float& t0, float& t1);

/**
 * @brief Compute the intersection of a ray with a triangle in 2d
 * @param ray The ray to intersect
 * @param p0 The first vertex of the triangle
 * @param p1 The second vertex of the triangle
 * @param p2 The third vertex of the triangle
 * @param tMin The fraction along the ray to the first intersection point
 * @param tMax The fraction along the ray to the second intersection point
 * @param minEdgeNr The edge number of the first intersection point
 * @param uMin The barycentric coordinate of the first intersection point
 * @param maxEdgeNr The edge number of the second intersection point
 * @param uMax The barycentric coordinate of the second intersection point
 * @return True if the ray intersects the triangle, false otherwise
 */
MESHTOOLS_EXPORT bool rayTriangleIntersection2d(const Ray2& ray,
                                                const Vec2& p0,
                                                const Vec2& p1,
                                                const Vec2& p2,
                                                float& tMin,
                                                float& tMax,
                                                int& minEdgeNr,
                                                float& uMin,
                                                int& maxEdgeNr,
                                                float& uMax);

/**
 * @brief Compute the intersection of a ray with a triangle in 3d when observed from a given direction
 * Calls rayTriangleIntersection2d after projecting the 3d points to 2d along the given projection dimension
 * @see rayTriangleIntersection2d.
 * @return True if the ray intersects the triangle, false otherwise
 */
MESHTOOLS_EXPORT bool rayTriangleIntersection2d(const Ray& ray,
                                                const Vec3& p0,
                                                const Vec3& p1,
                                                const Vec3& p2,
                                                float& tMin,
                                                float& tMax,
                                                int& minEdgeNr,
                                                float& uMin,
                                                int& maxEdgeNr,
                                                float& uMax,
                                                int projDim = 2);
} // namespace MeshTools
