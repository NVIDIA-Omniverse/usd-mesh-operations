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

#include "Arrays.h"
#include "Defines.h"
#include "Stage.h"
#include "Vec.h"

#include <memory>
#include <string>
#include <vector>


namespace MeshTools
{

class NavigationMesh;
class NavigationController;

#define MESHTOOLS_NO_GOAL Vec3(MaxFloat, MaxFloat, MaxFloat)

/**
 * @brief Parameters for creating debug lines in the construction process.
 */
struct NavigationMeshDebugParams
{
    /** If true, the boundary segments of the terrain are created.
     *  The segments the agent can cross are shown in yellow. The segments the agent cannot cross are shown in red. */
    bool getTerrainBoundary = false;

    bool getNavMeshNormals = false; ///< If true, the normals of the navigation mesh are created.
    bool getNavMeshFeatureCuts = false; ///< If true, cuts of the terrain features with the edges of the navigation mesh
                                        ///< are created.
    bool getSubdivisionPolygons = false; ///< If true, the polygons used to create the subdivison triangles are created.
    bool getNavMeshSubdivisionVertices = false; ///< If true, the subdivision vertices of the navigation mesh are
                                                ///< created.
    bool getNavMeshCreases = false; ///< If true, the creases of the navigation mesh are created.
};

/**
 * @brief Parameters for creating debug lines.
 */
struct NavigationControllerDebugParameters
{
    bool getObstacleBounds = false; ///< If true, the bounds of the obstacles are created.
    bool getObstacleDistanceField = false; ///< If true, the distance field of the obstacles is created.
    bool getBorderDistanceField = false; ///< If true, the distance field of the navigation mesh is created.
    bool getAreaDistanceField = false; ///< If true, the distance field of the navigation mesh is created.
    bool getAgentDistanceField = false; ///< If true, the distance field of the agents is created.
    bool getClearanceField = false; ///< If true, the clearance field of the navigation mesh is created.
    bool getVisibilityLines = false; ///< If true, the lines from the query point to all visible obstacles and agents
                                     ///< are created.
    bool getAgents = false; ///< If true, the agents are created.
    bool getAgentGoals = false; ///< If true, lines to the goals of the agents are created.
    bool getAgentPaths = false; ///< If true, the paths of the agents are created.

    /** @var agentId
     * If >= 0, the area distance field for the area costs of this agent is returned.
     * If < 0, the area distance field for the costs set by setAreaCosts() is returned.
     * If no call has been made to setAreaCosts(), the area distance field of the navigation mesh is returned.
     */
    int agentId = -1;

    /** @var relativeCrossSize
     * The size of the cross used to visualize distance fields.
     * The size is specified relative to the samplingDistance of the navigation mesh.
     */
    float relativeCrossSize = 0.25f;
};

/**
 * @brief Types of debug lines that can be created.
 */
enum class NavigationDebugLineType
{
    None,
    TerrainBoundaryBorder,
    TerrainBoundaryFeature,
    Normal,
    Cut,
    Polygon,
    Vertex,
    CreaseVertex,
    FixedVertex,
    BorderDistanceFieldVertex,
    AreaDistanceFieldVertex,
    ObstacleDistanceFieldVertex,
    AgentDistanceFieldVertex,
    ClearanceFieldVertex,
    CreaseEdge,
    ObstacleBounds,
    ObstructedEdgeByObstacle,
    ObstructedEdgeByAgent,
    ObstructedEdgeByArea,
    VisibilityLine,
    Agent,
    AgentGoal,
    AgentPath,
    AStarSearch,
    AstarPath,

    NumLineTypes
};

/**
 * @brief Describes lines for debug visualization.
 * The buffers can be used as they are in a glDrawArrays(GL_LINES, ..) call.
 * See NavigationMeshDebugParams and NavigationControllerDebugParameters for the debug lines that can be created.
 */
struct NavigationDebugLines
{
    void clear()
    {
        vertexPairs.clear();
        lineTypes.clear();
        objectIds.clear();
        firstLineOfType.clear();
    }

    MESHTOOLS_EXPORT void sort(); ///< Sorts the lines by type. The firstLineOfType array is updated accordingly.

    Vec3Array vertexPairs; ///< The debug lines as segments defined by pairs of vertices. The size is equal to the
                           ///< number of lines times two.
    Int32Array lineTypes; ///< One value of type NavigationDebugLineType for each line. The size is equal to the number
                          ///< of lines.
    Int32Array objectIds; ///< One id for each line. The object the line belongs to. The size is equal to the number of
                          ///< lines.
    Int32Array firstLineOfType; ///< The first line of each type. The size is equal to the number of types plus one.
    ///< This array is only valid after calling sort().
};

/**
 * @brief Parameters for creating a navigation mesh.
 */
struct NavigationMeshParameters
{
    /** @var useGpu
     * If true, the navigation mesh will be created on the GPU.
     */
    bool useGpu = false;

    /** @var minimumAgentRadius
     * The minimum radius of agents that will navigate the mesh.
     * This parameter determines the inset of the navmesh. The distance field is adjusted to this value
     * meaning the distance at the border of the navigation mesh is equal to the minimum agent radius.
     */
    float minimumAgentRadius = 0.1f;

    /** @var maximumAgentRadius
     * The maximum radius of agents that will navigate the mesh.
     * This parameter is used to determine the width of the distance field that is created to compute environment
     * proximity. Larger values will slow down the creation of the distance field.
     */
    float maximumAgentRadius = 1.0f;

    /** @var minimumClearance
     * The navigation mesh is only created in regions where the clearance is
     * greater than the minimum clearance. A clearance map is created which is used at runtime to determine whether
     * an agent equal or larger than the minimum clearance can navigate the mesh.
     */
    float minimumClearance = 2.0f;

    /** @var maxStepHeight
     * The maximal height of an obstacle the agent can step over.
     */
    float maxStepHeight = 0.4f;

    /** @var maxStepLength
     * The maximal width of a gap the agent can cross.
     */
    float maxStepLength = 0.5f;

    /** @var maxSlope
     * The maximum slope of the terrain in degrees that the agents can navigate.
     */
    float maxSlope = 45.0f;

    /** @var samplingDistance
     * The distance with which the terrain is sampled to create the initial vertices of the navmesh.
     */
    float samplingDistance = 0.2f;

    /** @var smoothVertices
     * If true, smooths out the positions of the vertices.
     */
    bool smoothVertices = true;

    /** @var snapVerticesToBorder
     * If true, the border of the navigation mesh will match the terrain boundary.
     * Otherwise, the vertices of the navigation mesh stay at their regularly sampled locations.
     */
    bool snapVerticesToBorder = true;

    /** @var smoothBorder
     * If true, smoothes the part of the border that was not snapped.
     */
    bool smoothBorder = false;

    /** @var subdivideTriangles
     * If true, triangles are subdivided allowing the navigation mesh to match sharp features of
     * the terrain.
     */
    bool subdivideTriangles = true;

    /** @var minimalTriangleSize
     * The minimal size of a triangle. If a triangle is smaller than this value, it is ignored
     * for the navMesh creation. The size is specified relative to the samplingDistance.
     */
    float minimalTriangleSize = 0.01f;

    /** @var upDimension
     * The dimension of the up direction. If -1, the up direction will be automatically computed.
     */
    int upDimension = -1;

    /** @var maxVerticesPerTile
     * The maximal number of vertices per tile. If the number of vertices of the input mesh
     * exceeds this value, it is split into tiles and the resulting navigation meshes stitched together at the end. This
     * is useful to bound the memory consumption during the creation of the navigation mesh. A value of 0 means that the
     * mesh is not split into tiles.
     */
    int maxVerticesPerTile = 10000000;

    /** @var decimateMesh
     * If true, the mesh is decimated to the target edge length after creation.
     */
    bool decimateMesh = false;

    /** @var decimationTargetEdgeLength
     * The target edge length of the decimated mesh.
     */
    float decimationTargetEdgeLength = 0.40f;

    /** @var minimumIslandDiameter
     * The minimum diameter of an island to be kept.
     * Islands are connected pieces of the navigation mesh. If an island has a smaller diameter
     * it is removed to prevent the creation of many little islands. In a typical scenario, only the largest island is
     * of interest. If the minimumIslandDiameter is larger than the diameter of the largest island, the largest island
     * is kept.
     */
    float minimumIslandDiameter = 1000.0f;

    /** @var borderPadding
     * The distance a agent is pushed away from the border of the nav mesh.
     * The distance is defined relative to the sampling distance.
     */
    float borderPadding = 0.1f;

    /** @var createDebugLines
     * If true, debug lines are created.
     * Creates debug lines that can be retrieved via the method getDebugLines.
     * Not all types are supported by the CPU version.
     * Subdivision related types are only created when subdivision is turned on.
     */
    bool createDebugLines = false;

    /** @var debugParameters
     * Specifies which type of debug lines are created.
     */
    NavigationMeshDebugParams debugParameters;
};


/**
 * @brief Parameters for creating a navigation controller.
 */
struct NavigationControllerParameters
{
    /** @var obstaclePadding
     * The distance a agent is pushed away from obstacles.
     * The distance is defined relative to the sampling distance.
     */
    float obstaclePadding = 0.1f;

    /** @var obstaclePaddingHeight
     * The cost to be closer than the agent's radius to an obstacle or the border of the navigation mesh.
     * The cost is defined relative to the sampling distance.
     */
    float radiusCost = 100.0f;

    /** @var gapCost
     * The cost to be closer than the agent's radius plus the gap to an obstacle or the border of the navigation mesh.
     * The cost is defined relative to the sampling distance.
     */
    float gapCost = 1.0f;

    /** @var agentCost
     * Cost to be close to other agents
     * The cost is defined relative to the sampling distance.
     */
    float agentCost = 1.0f;

    /** @var clearanceCost
     * The cost to be in a location with less clearance than the agent's height.
     * The cost is defined relative to the sampling distance.
     */
    float clearanceCost = 100.0f;

    /** @var blockedPathWaitFrames
     * The number of frames to wait when an agent's path is blocked before trying to compute a new path.
     */
    int blockedPathWaitFrames = 30;

    /** @var fullSearchFrameCount
     * The number of frames after which a complete A* search is performed.
     */

    int fullSearchFrameCount = 100;
};

/**
 * @brief Data for creating or updating multiple agents.
 * Used by NavigationController::createAgents() and NavigationController::updateAgents().
 * If a pointer is nullptr, the corresponding attribute is not changed (for updateAgents) or a default value is used
 * (for createAgents).
 */

struct NavigationAgentsData
{
    /** @brief The number of agents to create. */
    int numAgents = 0;

    /** @brief The IDs of the agents. */
    const int* ids = nullptr;

    /** @brief The positions of the agents. */
    const Vec3* positions = nullptr;

    /** @brief The velocities of the agents. */
    const float* velocities = nullptr;

    /** @brief The radii of the agents. */
    const float* radii = nullptr;

    /** @brief The collision gaps of the agents. This parameter is used by computeAgentCollisionCorrections() and
    simulateAgents(). These methods try to keep two agents away from each other by the sum of their radii plus the sum
    of their collision gaps */
    const float* collisionGaps = nullptr;

    /** @brief The heights of the agents. */
    const float* heights = nullptr;

    /** @brief The goal positions of the agents. A value of MESHTOOLS_NO_GOAL indicates that the agent does not follow a
     * goal. */
    const Vec3* goalPositions = nullptr;

    /** @brief The area costs of the agents. Each agent can have its own area costs for navigation.
    A value of 0.0 means that the cost of the navmesh is used.
    If not nullptr, a float array of costs of the size numAgents * numAreas is expected.*/
    const float* areaCosts = nullptr;

    /** @brief Makes certain agents ignore obstacles.
     * If not nullptr, a bool array of size numAgents is expected.*/
    const bool* ignoreObstacles = nullptr;
};


/**
 * @brief Represents the shape of an agent.
 *
 */
struct AgentShape
{
    AgentShape(float height = 0.0f, float radius = 0.0f, float collisionGap = 0.0f)
        : height(height), radius(radius), collisionGap(collisionGap)
    {
    }
    float height;
    float radius;
    float collisionGap;
};

/**
 * @brief Describes curves that are used to create the navigation mesh.
 *
 */
struct CurvesDescriptor
{
    const Vec3* points = nullptr;
    const float* width = nullptr;
    const int* lengths = nullptr;
    int numCurves = 0;
    int areaNr = 1; ///< Area number for triangles inside/on the curve (SDF <= 0)
};

/**
 * @brief Describes the baked state of a navigation mesh.
 * This matches the payload stored by serialize() / deserialize().
 */
struct NavigationMeshDescriptor
{
    Quat rotation = Quat(Identity);
    float samplingDistance = 0.0f;
    float maximumAgentRadius = 0.0f;

    std::vector<Vec3> vertices;
    std::vector<int> triangleIds;
    std::vector<float> triangleBorderDistances;
    std::vector<float> triangleAreaBorderDistances;
    std::vector<float> triangleClearances;
    std::vector<int> firstTriangleOfArea;
    std::vector<float> areaCosts;
    std::vector<std::string> areaNames;

    int vertexCount() const
    {
        return (int)vertices.size();
    }

    int triangleCount() const
    {
        return (int)triangleIds.size() / 3;
    }

    int areaCount() const
    {
        return (int)areaNames.size();
    }
};


/**
 * @brief Represents a path as a series of points.
 *
 */
class NavigationPath
{
public:
    /**
     * @brief Creates a new path.
     */
    MESHTOOLS_EXPORT NavigationPath();

    MESHTOOLS_EXPORT ~NavigationPath();

    /**
     * @brief Creates a navigation path from a set of points.
     * @param points The points that define the path.
     * @param numPoints The number of points in the path.
     */
    MESHTOOLS_EXPORT bool create(const Vec3* points, int numPoints);

    /**
     * @brief Returns the number of points in the path.
     * @return The number of points in the path.
     */
    MESHTOOLS_EXPORT size_t pointCount();

    /**
     * @brief Returns the points of the path.
     * @return The points of the path.
     */
    MESHTOOLS_EXPORT const Vec3* points() const;

    /**
     * @brief Returns the distance to the closest point on the path.
     * @param position The position to compute the distance from.
     * @param closestPoint The closest point on the path. If nullptr, the closest point is not returned.
     * @param tangent The tangent of the path at the closest point. If nullptr, the tangent is not returned.
     * @return The distance of the path.
     */
    MESHTOOLS_EXPORT float getClosestDistance(const Vec3& position,
                                              Vec3* closestPoint = nullptr,
                                              Vec3* tangent = nullptr) const;

    /**
     * @brief Returns the total length of the path.
     * @return The total length of the path.
     */
    MESHTOOLS_EXPORT float length();

    /**
     * @brief Returns the interpolated position on the path at a given distance along the path.
     * @param distance The distance along the path.
     * @param curved If true, the path is interpolated with a curve of a sequence of splines.
     * @return The interpolated position on the path.
     */
    MESHTOOLS_EXPORT Vec3 evaluatePosition(float distance, bool curved = false) const;

    /**
     * @brief Returns the tangent of the path at a given distance along the path.
     * @param distance The distance along the path.
     * @param curved If true, the path is interpolated with a curve of a sequence of splines.
     * @return The tangent of the path.
     */
    MESHTOOLS_EXPORT Vec3 evaluateTangent(float distance, bool curved = false) const;

    /*! \cond PRIVATE */
protected:
    class NavigationPathImpl;
    MESHTOOLS_EXPORT NavigationPath(const NavigationPathImpl&);
    std::unique_ptr<NavigationPathImpl> m_impl;
    friend class NavigationMesh;
    friend class NavigationController;
    /*! \endcond */
};


/**
 * @brief Represents a navigation mesh. This is the union of locations, an agent can navigate.
 *
 */
class NavigationMesh
{
public:
    /**
     * @brief Creates a new navigation mesh creator.
     */
    MESHTOOLS_EXPORT NavigationMesh();
    MESHTOOLS_EXPORT ~NavigationMesh();

    /**
     * @brief Creates a navigation controller that uses this navigation mesh.
     * @return A shared pointer to the navigation controller.
     */
    MESHTOOLS_EXPORT std::shared_ptr<NavigationController> createController(const NavigationControllerParameters& params);

    /**
     * @brief Creates a navigation mesh from a stage.
     * @param params The parameters for creating the navigation mesh.
     * @param stage The stage to create the navigation mesh from.
     * @param areaCosts The cost of each area for shortest path computation.
     * A negative cost means an inifite cost meaning that the agent will not step on this area.
     * @param numAreas The number of areas in the stage.
     * @param inclusionBounds A list of bounds defined by
     * consecutive minimum and maximum points.
     * If the pointer is not null, vertices are only created within these bounds.
     * Exclusion is stronger than inclusion. If a vertex is in both an inclusion and an exclusion bound, it is excluded.
     * @param inclusionBoundsAreas A list of areas for the inclusion bounds. Triangles having area -1 will get the area
     * of the inclusion bounds that contains them.
     * @param numInclusionBounds The number of inclusion bounds.
     * @param exclusionBounds A list of bounds defined by consecutive minimum and maximum points.
     * If the pointer is not null, no vertices are created within these bounds.
     * Exclusion is stronger than inclusion. If a vertex is in both an inclusion and an exclusion bound, it is excluded.
     * @param numExclusionBounds The number of exclusion bounds.
     * @param curveWidth Default width used for all stage curves (stage curves have no width data).
     * @param curveAreaNr Area number for triangles inside/on the curve.
     */
    MESHTOOLS_EXPORT bool create(const NavigationMeshParameters& params,
                                 const Stage& stage,
                                 const float* areaCosts = nullptr,
                                 int numAreas = 0,
                                 Vec3* inclusionBounds = nullptr,
                                 int* inclusionBoundsAreas = nullptr,
                                 int numInclusionBounds = 0,
                                 Vec3* exclusionBounds = nullptr,
                                 int numExclusionBounds = 0,
                                 float curveWidth = 1.0f,
                                 int curveAreaNr = 1);

    /**
     * @brief Creates a navigation mesh from a flattened stage.
     * @param params The parameters for creating the navigation mesh.
     * @param vertices The vertices of the input mesh.
     * @param numVertices The number of vertices.
     * @param triangleIds The flattened triangle indices of the input mesh.
     * @param triangleAreas The area is an integer value. In the shortest path computation, each area has its own
     * cost associated with it. The length of this array must be equal to the number of triangles.
     * If the pointer is nullptr, the area of all treangles is set to 0.
     * Triangles having area -1 will get the area of the inclusion bounds that contains them.
     * @param numTriangles The number of triangles.
     * @param areaNames The names of the areas in the stage. If the pointer is nullptr, the names of the areas are set
     * to "Area0", "Area1", ...
     * @param areaCosts The cost of each area for shortest path computation.
     * A negative cost means an inifite cost meaning that the agent will not step on this area.
     * @param numAreas The number of areas in the stage.
     * @param inclusionBounds A list of bounds defined by consecutive minimum and maximum points.
     * If the pointer is not null, vertices are only created within these bounds.
     * Exclusion is stronger than inclusion. If a vertex is in both an inclusion and an exclusion bound, it is excluded.
     * @param inclusionBoundsAreas A list of areas for the inclusion bounds. Triangles having area -1 will get the area
     * of the inclusion bounds that contains them.
     * @param numInclusionBounds The number of inclusion bounds.
     * @param exclusionBounds A list of bounds defined by consecutive minimum and maximum points.
     * If the pointer is not null, no vertices are created within these bounds.
     * Exclusion is stronger than inclusion. If a vertex is in both an inclusion and an exclusion bound, it is excluded.
     * @param numExclusionBounds The number of exclusion bounds.
     * @param curves The curves that are used to create the navigation mesh.
     * If nonzero, a special area is created along the curves.
     */
    MESHTOOLS_EXPORT bool create(const NavigationMeshParameters& params,
                                 const Vec3* vertices,
                                 int numVertices,
                                 const int* triangleIds,
                                 const int* triangleAreas,
                                 int numTriangles,
                                 const char** areaNames,
                                 const float* areaCosts,
                                 int numAreas,
                                 Vec3* inclusionBounds = nullptr,
                                 int* inclusionBoundsAreas = nullptr,
                                 int numInclusionBounds = 0,
                                 Vec3* exclusionBounds = nullptr,
                                 int numExclusionBounds = 0,
                                 const CurvesDescriptor* curves = nullptr);

    /**
     * @brief Sets the area costs for the navigation mesh.
     * @param areaCosts The cost of each area for shortest path computation.
     * A negative cost means an inifite cost meaning that the agent will not step on this area.
     * @param numCosts The number of costs provided.
     */
    MESHTOOLS_EXPORT void setAreaCosts(const float* areaCosts, int numCosts);

    /**
     * @brief Defines a callback function that is called during the creation of the navigation mesh.
     *
     * @param progress The amount of progress between 0.0 - 1.0
     * @param userData The user data passed to the @ref setProgressCallbackFunction
     * @return True if the creation should continue, false if it should be aborted.
     */

    typedef bool (*ProgressCallbackFunction)(float progress, void* userData);

    /**
     * @brief Sets a callback function that is called during the creation of the navigation mesh.
     * This function must be called before create is called
     * @param function This is called when any progress is made. True if the creation should continue, false if it
     * should be aborted.
     * @param userData The user data passed to the callback function.
     */
    MESHTOOLS_EXPORT void setProgressCallbackFunction(ProgressCallbackFunction function, void* userData = nullptr);

    /**
     * @brief Sets a callback function that is called during the creation of the navigation mesh.
     * This function must be called before create is called.
     * The function is used to get information about the creation process for debugging purposes.
     * @param function This is called when any progress is made.
     * @see LogCallbackFunction in Defines.h
     * @param userData The user data passed to the callback function.
     */
    MESHTOOLS_EXPORT void setLogCallbackFunction(LogCallbackFunction function, void* userData = nullptr);


    /**
     * @brief Returns the size of the navigation mesh after serialization.
     * @return The size of the navigation mesh after serialization.
     */
    MESHTOOLS_EXPORT size_t getSerializationSize() const;

    /**
     * @brief Serializes the navigation mesh to a binary buffer.
     * @param data The binary buffer that contains the navigation mesh after serialization.
     * @param size The size of the binary buffer.
     * @return False if the size of the buffer is too small, i.e. smaller than getSerializationSize().
     */
    MESHTOOLS_EXPORT bool serialize(const uint8_t* data, size_t size) const;

    /**
     * @brief Deserializes a navigation mesh from a binary buffer.
     * @param data The binary buffer containing the navigation mesh.
     * @param size The size of the binary buffer.
     * @return True if the navigation mesh was loaded successfully, false otherwise.
     */
    MESHTOOLS_EXPORT bool deserialize(const uint8_t* data, size_t size);

    /**
     * @brief Creates a navigation mesh from a descriptor.
     * @param descriptor The baked navigation mesh state.
     * @return True if the navigation mesh was created successfully, false otherwise.
     */
    MESHTOOLS_EXPORT bool createFromDescriptor(const NavigationMeshDescriptor& descriptor);

    /**
     * @brief Returns the baked navigation mesh state as a descriptor.
     * @param descriptor The descriptor to fill with the navigation mesh state.
     */
    MESHTOOLS_EXPORT void getDescriptor(NavigationMeshDescriptor& descriptor) const;

    /**
             * @brief Computes the shortest path on the navigation mesh between two points.
             * @param start The start point.
             * @param end The end point.
             * @param path The path that is computed.
             * @param agentShape The shape of the agent. This radius will determine the width of passages the agent can
       travel and
             * how close it can be to the environment. The height will determine the minimum clearance the agent needs
             to be allowed to move to a given location.
             * @param areaCosts The costs of each area. If nullptr, the costs are set to 1.0 for all areas.
             * @param straighten If true, the path is straightened. Otherwise it follows the cetners of triangle edges.
             * @param previousPath The path to be used as a hint for the path computation.
             * If present, the path is used to speed up the path computation by using temporal coherence.
             * The algorithm only searches for a path that is close to the previous path.
             * Only if no path is found, a full search is performed.
         * @param debugLines If not nullptr, the debug lines are filled with path computation information such as the
       triangles visited by the A* algorithm.
             * @return True if a path was found, false otherwise.
             */
    MESHTOOLS_EXPORT bool findShortestPath(const Vec3& start,
                                           const Vec3& end,
                                           NavigationPath& path,
                                           const AgentShape& agentShape,
                                           float* areaCosts = nullptr,
                                           bool straighten = true,
                                           const NavigationPath* previousPath = nullptr,
                                           NavigationDebugLines* debugLines = nullptr) const;

    /**
     * @brief Returns a random position on the navigation mesh.
     * @param randomPosition The random position on the navigation mesh.
     * @param agentShape The random position is guaranteed to be at least the distance of the radius away
     * from the border of the navigation mesh. The height determines the minimum clearance the agent needs
     to be allowed to move to a given location.
     * @param randomNumber A random number between 0 and 1 provided by the user.
     * This gives the user control of seeding and determinism.
     * If a negative number is provided, a random number will be generated internally. See setSeed.
     * @param The probability of each area. If null the point is picked ranomly independent of areas
     * If false, a position is picked randomply from the whole navigation mesh without considering areas.
     * @return True if a random position was found, false otherwise.
     */
    MESHTOOLS_EXPORT bool getRandomPosition(Vec3& randomPosition,
                                            const AgentShape& agentShape,
                                            float randomNumber = -1.0f,
                                            const float* areaProbabilities = nullptr) const;

    /**
     * @brief Returns a random position on the navigation mesh.
     * @param areas The areas the closest position should be from.
     * @param numAreas The number of areas provided. If this value is zero, the function will return the closest
     * position from any area.
     * @param queryPosition The position to query the closest position from.
     * @param position The closest position on the navigation mesh.
     * @param agentShape The random position is guaranteed to be at least the distance of the radius away
     * from the border of the navigation mesh. The height determines the minimum clearance the agent needs
     to be allowed to move to a given location.
     * @param searchIslandNr The island number to search for the closest position. If -1 all islands are searched.
     * @param positionIslandNr The island number of the closest position. If nullptr, the island number is not returned.
     * @return True if a closest position was found, false otherwise.
     */
    MESHTOOLS_EXPORT bool getClosestPosition(const Vec3& queryPosition,
                                             Vec3& closestPosition,
                                             const int* areas,
                                             int numAreas,
                                             const AgentShape& agentShape,
                                             int searchIslandNr = -1,
                                             int* positionIslandNr = nullptr) const;
    /**
     * @brief Returns the vertices of the navigation mesh.
     * @param vertices The vector to store the vertices in.
     */
    MESHTOOLS_EXPORT void getVertices(Vec3Array& vertices) const;

    /**
     * @brief Returns the number of vertices of the navigation mesh.
     * @return The number of vertices of the navigation mesh.
     */
    MESHTOOLS_EXPORT int vertexCount() const;

    /**
     * @brief Returns the triangle indices of the navigation mesh.
     * @param areaNr The area number of the triangles to return. A negative value will return all triangles.
     * @param triangleIds The vector to store the triangle indices in.
     */
    MESHTOOLS_EXPORT void getTriangleIds(int areaNr, Int32Array& triangleIds) const;

    /**
     * @brief Returns the number of triangles of the navigation mesh.
     * @param areaNr The area number of the triangles to return. A negative value will return all triangles.
     * @return The number of triangles of the navigation mesh.
     */
    MESHTOOLS_EXPORT int triangleCount(int areaNr = -1) const;

    /**
     * @brief Returns the first triangle of each area.
     * @param firstTriangleOfArea The vector to store the first triangle of each area in.
     * The length of the vector is equal to the number of areas plus one.
     * This way the number of triangles of the area i can be computed as firstTriangleOfArea[i + 1] -
     * firstTriangleOfArea[i].
     */
    MESHTOOLS_EXPORT void getFirstTriangleOfArea(Int32Array& firstTriangleOfArea) const;

    /**
     * @brief Returns the number of areas of the navigation mesh.
     * @return The number of areas of the navigation mesh.
     */
    MESHTOOLS_EXPORT int getAreaCount() const;

    /**
     * @brief Returns the name of an area.
     * @param areaNr The number of the area.
     * @return The name of the area.
     */
    MESHTOOLS_EXPORT const char* getAreaName(int areaNr) const;

    /**
     * @brief Returns the cost of an area.
     * @param areaNr The number of the area.
     * @return The cost of the area. If the area does not exist, returns 0.0f.
     */
    MESHTOOLS_EXPORT float getAreaCost(int areaNr) const;

    /**
     * @brief Computes the height of the navigation mesh at a given position.
     * @param position The position to compute the height at.
     * @param height The height of the navigation mesh at the given position.
     * @return True if the position lies inside the navigation mesh, false otherwise.
     */
    MESHTOOLS_EXPORT bool getHeightAt(const Vec3& position, float& height) const;

    /**
     * @brief Returns the up dimension of the navigation mesh.
     * @return The up dimension of the navigation mesh.
     */
    MESHTOOLS_EXPORT int getUpDimension() const;

    /**
     * @brief Returns debug lines for visualization.
     * Lines are only created if createDebugLines in NavigationMeshParameters is set to true before creation.
     * See NavigationMeshParameters::NavigationMeshDebugParams for the debug lines that can be created.
     */
    MESHTOOLS_EXPORT void getDebugLines(NavigationDebugLines& lines) const;

    /**
     * @brief Returns the number of triangles to draw.
     * @param areaNr The area number of the triangles to return. A negative value will return all triangles.
     * @return The number of triangles that will be drawn. This tells the user how many triangles to allocate in the
     * draw buffer.
     */
    MESHTOOLS_EXPORT int getDrawTriangleCount(int areaNr) const;

    /**
     * @brief Returns the triangles to draw.
     * @param areaIndex The area index of the triangles to draw.
     * @param vertices The buffer to hold the vertices of the triangles.
     * @param vertexStride The stride of vertices. The number of bytes used for the vertex plus padding.
     * For a consecutive array of Vec3, this value must be set to sizeof(Vec3).
     * @param triangleCount The number of triangles to draw.
     * @return The number of triangles drawn.
     */
    MESHTOOLS_EXPORT int getDrawTriangles(int areaNr, Vec3* vertices, int vertexStride, int triangleCount) const;

    /**
     * @brief Returns the number of lines to draw.
     * @param borderOnly If true, only the border lines are drawn.
     * @return The number of lines to draw.
     */
    MESHTOOLS_EXPORT int getDrawLineCount(bool borderOnly) const;

    /**
     * @brief Returns the lines to draw.
     * @param borderOnly If true, only the border lines are drawn.
     * @param linesFirstPoints The buffer to hold the first points of the lines.
     * @param linesSecondPoints The buffer to hold the second points of the lines.
     * @param pointStride The stride of points. The number of bytes used for the point plus padding.
     * For a consecutive array of Vec3, this value must be set to sizeof(Vec3).
     * @param drawLinesCount The number of lines to draw.
     * @return The number of lines drawn.
     */
    MESHTOOLS_EXPORT int getDrawLines(bool borderOnly,
                                      Vec3* linesFirstPoints,
                                      Vec3* linesSecondPoints,
                                      int linesPointStride,
                                      int drawLinesCount) const;

    /**
     * @brief Returns the signature of the navigation mesh.
     * The signature is a hash value that can be used test whether two navigation meshes are equal in unit tests.
     * Since GPU computations are non-deterministic, comparing the coordinates of the vertices to full precision is too
     * restrictive. Instead, two meshes are considered equal if their signatures are equal.
     * @return The signature of the navigation mesh.
     */
    MESHTOOLS_EXPORT uint32_t getMeshSignature() const;

    friend class NavigationController;

    /**
     * @brief Sets the seed for the random number generator used in getRandomPosition.
     * @param seed The seed for the random number generator.
     */
    MESHTOOLS_EXPORT void setSeed(unsigned int seed);


    /*! \cond PRIVATE */
protected:
    class NavigationMeshImpl;
    MESHTOOLS_EXPORT NavigationMesh(const NavigationMeshImpl&);
    std::unique_ptr<NavigationMeshImpl> m_impl;
    /*! \endcond */
};


/**
 * @brief Represents a navigation controller which handles dynamic agents and obstacles on a navigation mesh.
 *
 */
class NavigationController
{
public:
    /**
     * @brief Creates a new navigation mesh creator.
     */
    MESHTOOLS_EXPORT NavigationController();
    MESHTOOLS_EXPORT ~NavigationController();


    /**
     * @brief Create debug lines for visualization.
     * @param params Defines which debug lines are created
     * @param debugLines The debug lines, see NavigationDebugLines.
     * @param maxAgentDistance Only debug lines within a maximum distance of maxAgentDistance to any agent are created.
     * If maxAgentDistance is zero, all debug lines are created.
     */
    MESHTOOLS_EXPORT void createDebugLines(const NavigationControllerDebugParameters& params,
                                           NavigationDebugLines& debugLines,
                                           float maxAgentDistance = 0.0f) const;
    /**
     * @brief Creates agents.
     * @param data The data for creating the agents (see NavigationAgentsData).
     * @return True if all agents have been created. False if some agents already exist.
     */
    MESHTOOLS_EXPORT bool createAgents(const NavigationAgentsData& data);

    /**
     * @brief Creates agents from a stage for debug purposes.
     * @param stage The stage to create the agents from.
     * One agent is created for each mesh in the stage whose name contains the substring "agent<Id>" where <Id> is the
     * integer id of the agent.
     * Area costs can be specified with a substring "costs<string>" where <string> is a sequece characters '0'-'9', 'x'.
     * Each character specifies the cost for the areas in order. '0'-'9' specify costs 0.0 to 9.0, 'x' specifies -1.0
     * (forbidded). As specified above '0' uses the cost of the navigation mesh. If a mesh with then name "goal<Id>"
     * exists, the position of that mesh is used as the goal position of the agent.
     * @return The largest agent id, -1 if no agent was created
     */
    MESHTOOLS_EXPORT int createAgents(const Stage& stage);

    /**
     * @brief Updates the agents.
     * @param data The data for updating the agents (see NavigationAgentsData).
     * @return True if some agents were moved. False if all positions are the same as before.
     */
    MESHTOOLS_EXPORT bool updateAgents(const NavigationAgentsData& data);

    /**
     * @brief Destroys agents.
     * @param ids The ids of the agents to destroy. If nullptr, all agents are destroyed.
     * @param numAgents The number of agent to be destroyed.
     * @return True if all agents were destroyed. False if some agents do not exist.
     */
    MESHTOOLS_EXPORT bool destroyAgents(const int* ids, int numAgents);

    /**
     * @brief sets the obstacles that the agent ignores.
     * @param agentId The id of the agent.
     * @param objstacleIds The ids of the obstacles that the agent ignores.
     * @param numObstacleIds The number of obstacles that the agent ignores.
     * @return True if the agent exist, false otherwise.
     */
    MESHTOOLS_EXPORT bool setAgentIgnoredObstacles(int agentId, int* objstacleIds, int numObstacleIds);

    /**
     * @brief Returns the ids of the agents near a position.
     * @param position The position to check from.
     * @param ids The ids of the agents near a position
     * @param maximalDistance The maximal distance to check for agents.
     * @return The number of agents returned.
     */
    MESHTOOLS_EXPORT int getNearAgents(const Vec3& position, Int32Array& ids, float maximalDistance) const;

    /**
     * @brief Simulates the agents requesting to find the next shortest position on this navmesh reach their goals
     * whilst avoiding other agents and obstacles.
     * @param time The delta time (in seconds)..
     * @param handleAgentPairCollisions If true, inter-agent collisions are considered during the simulation.
     * In this case, agents will push each other away to avoid collisions. If false, agents can overlap.
     * @param debugLines If not nullptr, the debug lines are filled with path computation information such as the
     * triangles visited by the A* algorithm as in the findShortestPath methods
     */
    MESHTOOLS_EXPORT void simulateAgents(float time,
                                         bool handleAgentPairCollisions = true,
                                         NavigationDebugLines* debugLines = nullptr);

    /**
     * @brief Saves the current simulation state of the agents.
     */
    MESHTOOLS_EXPORT void saveSimulationState();

    /**
     * @brief Restores the simulation state of the agents.
     */
    MESHTOOLS_EXPORT void restoreSimulationState();

    /**
     * @brief Gets the simulation results of the agents next positions to move to.
     * @param ids The ids of the agents to get the simulation results for.
     * @param numAgents The number of agents.
     * @param nextPositions The array of next positions to move to.
     */
    MESHTOOLS_EXPORT void getSimulatedAgents(const int* ids, int numAgents, Vec3Array& nextPositions);

    /**
     * @brief Computes the paths of all agents based on their current positions and goal positions.
     * This method allows the user to simulate the agents in a custom way without the use of simulateAgents.
     * @param debugLines If not nullptr, the debug lines are filled with path computation information such as the
     * triangles visited by the A* algorithm as in the findShortestPath methods
     */
    MESHTOOLS_EXPORT void computeAgentPaths(NavigationDebugLines* debugLines = nullptr);

    /**
     * @brief Computes collision correction vectors for a set of agents.
     * This method allows the user to simulate the agents in a custom way without the use of simulateAgents.
     * @param agentIds The ids of the agents to compute the collision corrections for.
     * @param agentCount The number of agents.
     * @param corrections The array of corrections for each agent. The correction is a vector that
     * needs to be added to the agent's position to avoid collisions with other agents.
     */
    MESHTOOLS_EXPORT void computeAgentCollisionCorrections(const int* agentIds, int agentCount, Vec3* corrections);

    /**
     * @brief Gets the path of an agent computed by the method simulateAgents.
     * @param agentId The id of the agent
     * @param path The path computed  by the method simulateAgents.
     * @return True if the agent with the id exists, false otherwise
     */
    MESHTOOLS_EXPORT bool getAgentPath(int agentId, NavigationPath& path) const;

    /**
     * @brief Creates obstacles as dynamic meshes.
     * @param ids The ids of the obstacles.
     * @param numObstacles The number of obstacles to create.
     * @param transforms The transforms of the obstacles.
     * @param firstVertexOfObstacle The first vertex of each obstacle in the vertices array.
     * @param firstTriangleOfObstacle The first triangle of each obstacle in the triangleIds array.
     * @param vertices The vertices of the union of all obstacles.
     * @param numVertices The number of vertices in the union of all obstacles.
     * @param triangleIds The triangle indices relative to the first vertex of each obstacle.
     * @param numTriangles The number of triangles in the union of all obstacles.
     * obstacles. If zero, the agentRadius of the navigation mesh is used.
     * @param minimumResamplingTriangleCount or performance reasons
     * and to guarantee mesh quality and manifoldness, obstacles may be replaced by approximating meshes.
     * This parameter defines the minimum number of triangles that an obstacle must have to be resampled.
     * If the number of triangles is smaller than this value, the obstacle is not resampled.
     * If minimumResamplingTriangleCount is zero, all obstacles are resampled.
     * @param relativeSamplingDistance
     * This parameter defines the average length of the edges of the approximating meshes
     * relative the sampling distance of the navigation mesh.
     * If the value is zero, obstaces are not resampled.
     * @return True if all obstacles have been created. False if some obstacles already exist.
     */
    MESHTOOLS_EXPORT bool createObstacles(const int* ids,
                                          int numObstacles,
                                          const Mat44* transforms,
                                          const int* firstVertexOfObstacle,
                                          const int* firstTriangleOfObstacle,
                                          const Vec3* vertices,
                                          int numVertices,
                                          const int* triangleIds,
                                          int numTriangles,
                                          int minimumResamplingTriangleCount = 0,
                                          float relativeSamplingDistance = 1.0f);

    /**
     * @brief Creates obstacles from a stage for debug purposes.
     * This method creates one obstacle for each mesh in the stage whose name contains the substring "obstacle".
     * @param stage The stage to create the obstacles from.
     * @return True if any obstacles were created.
     */
    MESHTOOLS_EXPORT bool createObstacles(const Stage& stage);

    /**
     * @brief Updates the transforms of the obstacles.
     * @param ids The ids of the obstacles to transform.
     * @param numObstacles The number of obstacle to update.
     * @param transforms The new transforms of the obstacles.
     * @return True if some obstacles were updated. False if all transforms are the same as before.
     */
    MESHTOOLS_EXPORT bool updateObstacles(const int* ids, int numObstacles, const Mat44* transforms);

    /**
     * @brief Destroys obstacles.
     * @param ids The ids of the obstacles to destroy.
     * @param numObstacles The number of obstacle ids.
     * @return True if all obstacles were destroyed. False if some obstacles do not exist.
     */
    MESHTOOLS_EXPORT bool destroyObstacles(const int* ids, int numObstacles);

    /**
     * @brief Returns the obstacles ids of the obstacles near a position.
     * @param position The position to check from.
     * @param ids The ids of the obstacles near a position
     * @param maximalDistance The maximal distance to check for obstacles.
     * @return The number of obstacles returned.
     */
    MESHTOOLS_EXPORT int getNearObstacles(const Vec3& position, Int32Array& ids, float maximalDistance) const;

    /**
     * @brief Sets the area costs for the navigation mesh.
     * @param areaCosts The cost of each area for shortest path computation.
     * A negative cost means an inifite cost meaning that the agent will not step on this area.
     * A separate area distance field is created for each distinct set of area costs.
     * If the area costs provided have not been used before, a new area distance field is created.
     * Otherwise, the existing area distance field is used.
     * @param numCosts The number of costs provided.
     */
    MESHTOOLS_EXPORT void setAreaCosts(const float* areaCosts, int numCosts);

    /**
     * @brief Computes the shortest path on the navigation mesh between two points.
     * @param start The start point.
     * @param end The end point.
     * @param path The path that is computed.
             * @param agentShape The shape of the agent. This radius will determine the width of passages the agent can
     travel and
             * how close it can be to the environment. The collisionGap defines an additional preferred distance if a
     path exists. The height will determine the minimum clearance the agent needs to be allowed to move to a given
     location.
     * @param agentId The id of the agent that is used to compute the path.
     * If positive, the obstacles the agent ignores are considered. If -1, no obstacles are ignored.
     * @param straighten If true, the path is straightened. Otherwise it follows the cetners of triangle edges.
     * @param previousPath The path to be used as a hint for the path computation.
     * If present, the path is used to speed up the path computation by using temporal coherence.
     * The algorithm only searches for a path that is close to the previous path.
     * Only if no path is found, a full search is performed.
     * @param debugLines If not nullptr, the debug lines are filled with path computation information such as the
     triangles visited by the A* algorithm.
     * @return True if a path was found, false otherwise.
     */
    MESHTOOLS_EXPORT bool findShortestPath(const Vec3& start,
                                           const Vec3& end,
                                           NavigationPath& path,
                                           AgentShape agentShape,
                                           bool straighten = true,
                                           const NavigationPath* previousPath = nullptr,
                                           NavigationDebugLines* debugLines = nullptr) const;

    /**
     * @brief Returns a random position on the navigation mesh.
     * @param randomPosition The random position on the navigation mesh.
     * @param agentShape The random position is guaranteed to be at least the distance of the radius away
     * from the border of the navigation mesh and the obstacles. The height determines the minimum clearance the agent
     needs to be allowed to move to a given location.
     * @param agentId The id of the agent that is used to compute the path.
     * If positive, the obstacles the agent ignores are considered. If -1, no obstacles are ignored.
     * @param randomNumber A random number between 0 and 1 provided by the user.
     * This gives the user control of seeding and determinism.
     * If a negative number is provided, a random number will be generated internally.
     * @param The probability of each area. If null the point is picked ranomly independent of areas.
     * @return True if a random position was found, false otherwise.
     */
    MESHTOOLS_EXPORT bool getRandomPosition(Vec3& randomPosition,
                                            AgentShape agentShape,
                                            float randomNumber = -1.0f,
                                            const float* areaProbabilities = nullptr) const;

    /**
     * @brief Returns a random position on the navigation mesh.
     * @param areas The areas the closest position should be from.
     * @param numAreas The number of areas provided. If this value is zero, the function will return the closest
     * position from any area.
     * @param queryPosition The position to query the closest position from.
     * @param position The closest position on the navigation mesh.
     * @param agentShape The shape of the agent. The random position is guaranteed to be at least the distance of the
     radius away
     * from the border of the navigation mesh and the obstacles. The height determines the minimum clearance the agent
     needs to be allowed to move to a given location.
     * @param agentId The id of the agent that is used to compute the path.
     * If positive, the obstacles the agent ignores are considered. If -1, no obstacles are ignored.
     * @param searchIslandNr The island number to search for the closest position. If -1 all islands are searched.
     * @param positionIslandNr The island number of the closest position. If nullptr, the island number is not returned.
     * @return True if a closest position was found, false otherwise.
     */
    MESHTOOLS_EXPORT bool getClosestPosition(const Vec3& queryPosition,
                                             Vec3& closestPosition,
                                             const int* areas,
                                             int numAreas,
                                             AgentShape agentShape,
                                             int searchIslandNr = -1,
                                             int* positionIslandNr = nullptr) const;

    /**
     * @brief Sets the seed for the random number generator used in getRandomPosition.
     * @param seed The seed for the random number generator.
     */
    MESHTOOLS_EXPORT void setSeed(unsigned int seed);

    /**
     * @brief Starts profiling the navigation controller.
     */

    MESHTOOLS_EXPORT void startProfiling();

    /**
     * @brief Stops profiling the navigation controller.
     */

    MESHTOOLS_EXPORT void stopProfiling();

    /**
     * @brief Returns the profiling string.
     * @return The profiling string.
     */

    MESHTOOLS_EXPORT const char* getProfilingString() const;


    /*! \cond PRIVATE */
protected:
    class NavigationControllerImpl;
    MESHTOOLS_EXPORT NavigationController(const NavigationControllerImpl&);
    std::unique_ptr<NavigationControllerImpl> m_impl;
    friend class NavigationMesh;
    /*! \endcond */
};


}
