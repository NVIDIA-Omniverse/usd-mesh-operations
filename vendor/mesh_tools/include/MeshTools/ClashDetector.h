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
#include "Stage.h"
#include "Vec.h"
#include "Vecd.h"

#include <memory>

namespace MeshTools
{

/**
 * @brief Parameters for the clash detector.
 */
struct ClashDetectorParameters
{
    /** @var useGpu
     * If true, the clash detection will be performed on the GPU.
     */
    bool useGpu = true;

    /** @var useParallelCpuClashDetection
     * When useGpu is false, if true the CPU implementation uses multiple threads for point-clash
     * and face-intersection passes. If false, those passes run single-threaded (e.g. for reproducibility
     * or debugging). Ignored when useGpu is true.
     */
    bool useParallelCpuClashDetection = false;

    /** @var gpuId
     * The ID of the GPU to use for clash detection.
     */
    int gpuId = 0;

    /** @var useSimpleIntersectionDetection
     * If true, simple intersection detection will be used instead of the default method.
     * This method does neigher use instance clustering nor a two level BVH.
     * It can be used for small scenes as a reference implementation for debugging.
     */
    bool useSimpleIntersectionDetection = false;

    /** @var computeIntersections
     * If true, intersections between the meshes will be computed.
     */
    bool computeIntersections = true;

    /** @var computeIntersectionsDoublePrecision
     * If true, intersections between the meshes will be computed using double precision.
     * This is slower, but more accurate for large scenes.
     */
    bool computeIntersectionsDoublePrecision = false;

    /** @var computeClashes
     * If true, point clashes between the meshes will be computed.
     */
    bool computeClashes = true;

    /** @var computeClashesDoublePrecision
     * If true, point clashes between the meshes will be computed using double precision.
     * This is slower, but more accurate for large scenes.
     */
    bool computeClashesDoublePrecision = false;

    /** @var minCutDistance
     * The minimum relative distance of an edge cut to the edge endpoints.
     * Small values larger than zero avoid numerical issues when faces only touch.
     */
    float minCutDistance = 0.0f;

    /** @var samplingPointDistance
     * The distance between sampling points on the faces of the meshes.
     * Smaller values lead to more accurate results, but also increase the computation time.
     * If this value is zero, the vertices of the meshes are used as sampling points.
     */
    float samplingPointDistance = 0.0f;

    /** @var searchDistance
     * The maximum distance to search for clashes.
     */
    float searchDistance = 0.3f;

    /** @var minimumClashDepth
     * The minimal depth to consider a clash valid.
     * Negative values mean that clashes with a small gap are also reported.
     * The range is from -searchDistance to +searchDistance.
     */
    float minimumClashDepth = 0.0f;

    /** @var maximumNumberOfVerticesPerTile
     * Enables tiling of large scenes that do not fit into GPU memory.
     * If this value is zero, no tiling is performed.
     * If value is greater than zero, the scene is split into tiles
     * with approximately the given number of vertices per tile.
     * A typical value is 50 million vertices.
     */
    int maximumNumberOfVerticesPerTile = 0;
};

/**
 * @brief Descriptor for a mesh used in clash detection.
 */
struct ClashMeshDescriptor
{
    /** @brief The vertices of the mesh */
    const Vec3* vertices = nullptr;
    /** @brief The number of vertices in the mesh */
    int numVertices = 0;
    /** @brief The indices of the mesh */
    const int* indices = nullptr;
    /** @brief The number of indices in the mesh */
    int numIndices = 0;
    /** @brief The sizes of the faces of the mesh */
    const int* faceSizes = nullptr;
    /** @brief The number of faces in the mesh */
    int numFaces = 0;
    /** @brief The transform of the mesh.
     */
    Mat44d transform;
    /** @brief The group of the mesh. Meshes in the same group do not clash with each other. Meshes with group zero
     * clash with all other meshes. */
    int groupId = 0;
};

/**
 * @brief Information about a face intersection
 */
struct FaceIntersection
{
    /** @brief The mesh index of the first mesh */
    int meshId0;
    /** @brief The face index of the first mesh */
    int faceId0;
    /** @brief The mesh index of the second mesh */
    int meshId1;
    /** @brief The face index of the second mesh */
    int faceId1;
    /** @brief The collision depth of the two meshes involved in the intersection */
    float depth;
    /** @brief The frame number the intersection occurred */
    int frameNr = 0;
};

/**
 * @brief Filter for face intersections
 */
struct FaceIntersectionFilter
{
    /** @brief If >= 0, only intersections involving this mesh ID are reported */
    int firstMeshId = -1;

    /** @brief If firstMeshId >= 0 and secondMeshId >= 0, only intersections involving this mesh pair are reported */
    int secondMeshId = -1;

    /** @brief If >= 0, only intersections involving this face ID are reported */
    int firstFaceId = -1;

    /** @brief If firstFaceId >= 0 and secondFaceId >= 0, only intersections involving this face pair are reported */
    int secondFaceId = -1;

    /** @brief The bounding box to filter intersections */
    Bounds3 bounds = Bounds3(Empty);

    /** @brief The minimum depth of intersections to report */
    float minDepth = -MaxFloat;

    /** @brief The frame number to filter intersections. A negative value means no filtering. */
    int frameNr = -1;
};

/**
 * @brief Information about a point involved in a clash
 */
struct PointClash
{
    /** @brief The mesh index of the point */
    int meshId = -1;
    /** @brief The vertex index if vertices are used as points, -1 if not */
    int vertexId = -1;
    /** @brief The position of the point when the clash was detected*/
    Vec3 pointPosition = Vec3(Zero);
    /** @brief The penetration depth of the point into the closest mesh. Negative if outside the mesh. */
    float penetrationDepth = 0.0f;
    /** @brief The closest mesh to the point */
    int closestMesh = -1;
    /** @brief The closest surface point on the closest mesh */
    Vec3 closestSurfacePoint = Vec3(Zero);
    /** @brief The frame number the clash was detected */
    int frameNr = 0;
};

struct PointClashDoublePrecision
{
    /** @brief The mesh index of the point */
    int meshId = -1;
    /** @brief The vertex index if vertices are used as points, -1 if not */
    int vertexId = -1;
    /** @brief The position of the point when the clash was detected*/
    Vec3d pointPosition = Vec3d(Zero);
    /** @brief The penetration depth of the point into the closest mesh. Negative if outside the mesh. */
    float penetrationDepth = 0.0f;
    /** @brief The closest mesh to the point */
    int closestMesh = -1;
    /** @brief The closest surface point on the closest mesh */
    Vec3d closestSurfacePoint = Vec3d(Zero);
    /** @brief The frame number the clash was detected */
    int frameNr = 0;
};

/**
 * @brief A pair of clashing meshes
 */
struct ClashMeshPair
{
    /** @brief The first mesh involved in the clash */
    int mesh0;
    /** @brief The second mesh involved in the clash */
    int mesh1;
    /** @brief The vertex clash responsible for the maximum penetration, if -1 the clash is not caused by a vertex but
     * by face intersections. */
    int pointClashIndex;
    /** @brief The maximum penetration depth of the clash */
    float maximumPenetrationDepth;
    /** @brief The position of the vertex causing the maximum penetration */
    Vec3d maximumPointMesh0;
    /** @brief The position of the closest point on the other mesh */
    Vec3d maximumPointMesh1;
};


/**
 * @brief Finds clashes in a stage.
 *
 */
class ClashDetector
{

public:
    /// Constructs an empty clash detector
    MESHTOOLS_EXPORT ClashDetector();
    MESHTOOLS_EXPORT ~ClashDetector();

    /**
     * @brief Detects clashes among the provided meshes.
     * @param params The parameters for the clash detector
     * @param meshDescriptors The array of mesh descriptors
     * @param numMeshes The number of meshes
     */
    MESHTOOLS_EXPORT void detectClashes(const ClashDetectorParameters& params,
                                        const ClashMeshDescriptor* meshDescriptors,
                                        int numMeshes);

    /**
     * @brief Detects clashes with new transforms.
     * The clash detector identifies dynamic meshes by comparing the current and previous transforms.
     * Then, only dynamic meshes or meshes that are intersected by dynamic meshes are tested,
     * which is typically much faster than testing all meshes.
     * The frame number is incremented and the results are appended to the previous results with the new frame number.
     * @param params The parameters for the clash detector
     * @param meshDescriptors The array of mesh descriptors
     * @param numMeshes The number of meshes
     * @param appendResults If true, the results are appended to the previous results with the new frame number.
     */
    MESHTOOLS_EXPORT void updateClashes(const ClashDetectorParameters& params,
                                        const ClashMeshDescriptor* meshDescriptors,
                                        int numMeshes,
                                        bool appendResults = false);

    /**
     * @brief Initializes the clash detector with a stage.
     */
    MESHTOOLS_EXPORT void detectClashes(const ClashDetectorParameters& params, const Stage& stage);

    /**
     * @brief Updates the clash detector with a stage.
     * @param appendResults If true, the results are appended to the previous results with the new frame number.
     */
    MESHTOOLS_EXPORT void updateClashes(const ClashDetectorParameters& params,
                                        const Stage& stage,
                                        bool appendResults = false);

    /**
     * @brief Frees the resources used by the clash detector to store the results
     * This memory is used for querying the results of the last detection.
     * After this call, the results are no longer available.
     * The results are also freed when the clash detector is destroyed.
     */
    MESHTOOLS_EXPORT void freeResults();

    /**
     * @brief Returns the number of point clashes found in the last call to findClashes.
     */
    MESHTOOLS_EXPORT size_t getNumPointClashes() const;

    /**
     * @brief Returns the array of point clashes found in the last call to findClashes.
     * This is only available if computeClashesDoublePrecision was set to false in the ClashDetectorParameters.
     */
    MESHTOOLS_EXPORT const PointClash* getPointClashes() const;

    /**
     * @brief Returns the array of point clashes in double precision found in the last call to findClashes.
     * This is only available if computeClashesDoublePrecision was set to true in the ClashDetectorParameters.
     */
    MESHTOOLS_EXPORT const PointClashDoublePrecision* getPointClashesDoublePrecision() const;

    /**
     * @brief Returns the number of clash mesh pairs found in the last call to findClashes.
     */
    MESHTOOLS_EXPORT size_t getNumMeshClashes() const;

    /**
     * @brief Returns a list of all clash mesh pairs found in the last call to findClashes.
     * Each clash pair contains the two meshes that are clashing, the maximum penetration
     * and the two vertices that cause the maximum penetration.
     */
    MESHTOOLS_EXPORT const ClashMeshPair* getClashMeshPairs() const;

    /**
     * @brief Returns the clash mesh pair for the given two meshes if they are clashing.
     * @param meshId0 The first mesh index
     * @param meshId1 The second mesh index
     * @param pair The clash mesh pair if found
     * @return True if the meshes are clashing, false otherwise
     */
    MESHTOOLS_EXPORT bool getClashMeshPair(int meshId0, int meshId1, ClashMeshPair& pair) const;

    /**
     * @brief Returns the number of face intersections found in the last call to detectClashes.
     */
    MESHTOOLS_EXPORT size_t getNumFaceIntersections() const;
    /**
     * @brief Returns the array of face intersections found in the last call to detectClashes.
     */
    MESHTOOLS_EXPORT const FaceIntersection* getFaceIntersections() const;

    /**
     * @brief Returns the intersection segments found in the last call to detectClashes.
     * Each consecutive pair of points defines one segment. The size of the array is two times the number of face
     * intersections.
     * This is only available if computeIntersectionsDoublePrecision was set to false in the ClashDetectorParameters.
     */
    MESHTOOLS_EXPORT const Vec3* getFaceIntersectionSegments() const;

    /**
     * @brief Returns the intersection segments in double precision found in the last call to detectClashes.
     * Each consecutive pair of points defines one segment. The size of the array is two times the number of face
     * intersections.
     * This is only available if computeIntersectionsDoublePrecision was set to true in the ClashDetectorParameters.
     */
    MESHTOOLS_EXPORT const Vec3d* getFaceIntersectionSegmentsDoublePrecision() const;


    /**
     * @brief Retreives all the face intersections that match the given filter. They can be accessed using
     * getQueryFaceIntersections.
     * @param filter The filter to apply
     * @return True if any intersections match the filter, false otherwise
     */
    MESHTOOLS_EXPORT bool queryFaceIntersections(const FaceIntersectionFilter& filter);

    /**
     * @brief Returns the number of face intersection indices filtered by queryFaceIntersections.
     */
    MESHTOOLS_EXPORT size_t getNumQueryFaceIntersections() const;

    /**
     * @brief Returns a list of face intersections indices filtered by queryFaceIntersections.
     * These indices refer to the arrays returned by getFaceIntersections() and getFaceIntersectionSegments().
     * For the latter the segment end points are stored at locations 2 * index and 2 * index + 1.
     */
    MESHTOOLS_EXPORT const int* getQueryFaceIntersectionIds() const;

    /**
     * @brief Returns the number of sampling points generated on the faces of the meshes.
     * These points are used to detect clashes.
     */
    MESHTOOLS_EXPORT size_t getNumSamplingPoints() const;
    /**
     * @brief Returns the sampling points generated on the faces of the meshes.
     * These points are used to detect clashes.
     * For the GPU implementation, the points are retrieved the first time this function is called.
     * Therefore the function is not const.
     */
    MESHTOOLS_EXPORT const Vec3* getSamplingPoints();

    /**
     * @brief Defines a callback function that is called during the detection of clashes.
     *
     * @param progress The amount of progress between 0.0 - 1.0
     * @param userData The user data passed to the @ref setProgressCallbackFunction
     * @return True if the search should continue, false if it should be aborted.
     */

    typedef bool (*ProgressCallbackFunction)(float progress, void* userData);

    /**
     * @brief Sets a callback function that is called during the detection of clashes.
     * This function must be called before findClashes is called
     * @param function This is called when any progress is made. True if the search should continue, false if it
     * should be aborted.
     * @param userData The user data passed to the callback function.
     */
    MESHTOOLS_EXPORT void setProgressCallbackFunction(ProgressCallbackFunction function, void* userData = nullptr);

    /**
     * @brief Sets a callback function to get logging information.
     * @param function This is called when any progress is made.
     * @see LogCallbackFunction in Defines.h
     * @param userData The user data passed to the callback function.
     */
    MESHTOOLS_EXPORT void setLogCallbackFunction(LogCallbackFunction function, void* userData = nullptr);


    /*! \cond PRIVATE */
protected:
    class ClashDetectorImpl;
    MESHTOOLS_EXPORT ClashDetector(const ClashDetectorImpl&);
    std::unique_ptr<ClashDetectorImpl> m_impl;
    /*! \endcond */
};

}
