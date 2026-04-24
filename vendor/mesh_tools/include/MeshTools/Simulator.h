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

/** @brief Parameters describing the properties of a rigid body.
 * @param density The density of the material. If zero the body is considered to be static
 * @param thickness The thickness of the material. This is used for collision detection.
 * @param staticFrictionCoefficient The static friction coefficient of the material.
 * @param dynamicFrictionCoefficient The dynamic friction coefficient of the material.
 * @param restitution The restitution of the material.
 * @param globalDampingCoefficient The global damping coefficient of the body. This is used to simulate air resistance.
 * @param linearVelocity The initial linear velocity of the body.
 * @param angularVelocity The initial angular velocity of the body.
 * @param collisionGroups The collision groups of the body. This is used for collision detection.
 * @param attachmentGroups The attachment groups of the body. This is used for auto attaching the body to other objects.
 * @param visible If true the body is visible in the scene.
 */

struct RigidBodyParams
{
    float density = 1000.0f;
    float thickness = 0.01f; // Thickness of the material for collision detection
    float staticFrictionCoefficient = 1.0f;
    float dynamicFrictionCoefficient = 0.0f;
    float restitution = 0.0f;
    float globalDampingCoefficient = 0.0f;
    Vec3 linearVelocity = Vec3(Zero);
    Vec3 angularVelocity = Vec3(Zero);
    unsigned int collisionGroups = 0;
    unsigned int attachmentGroups = 0;
    bool visible = true;
};

/**
 * @brief Parameters describing the properties of a joint.
 */

enum class JointType
{
    NONE,
    DISTANCE,
    FIXED,
    HINGE,
    MOTOR,
    SERVO,
    BALL,
    PRISMATIC,
    CYLINDER
};


struct JointParams
{
    JointType type = JointType::NONE;

    // distance

    bool hasTargetDistance = false;
    float targetDistance = 0.0f;
    float distanceCompliance = 0.0f;
    float minDistance = -MaxFloat;
    float maxDistance = MaxFloat;
    float distanceDampingCoeff = 0.0f;

    // orientation

    float minSwing = -MaxFloat;
    float maxSwing = MaxFloat;
    float minTwist = -MaxFloat;
    float maxTwist = MaxFloat;

    Vec3 anglularVelocities = Vec3(Zero);
    bool hasTargetAngles = false;
    Vec3 targetAngles = Vec3(Zero);
    float angleCompliance = 0.0f;
    float orientationDampingCoeff = 0.0f;

    // motor
    float velocity = 0.0f;
    float minVelocity = -MaxFloat;
    float maxVelocity = MaxFloat;

    MESHTOOLS_EXPORT static JointParams makePrismaticJoint(bool hasLimits = false,
                                                           float minDistance = -MaxFloat,
                                                           float maxDistance = MaxFloat,
                                                           bool hasTarget = false,
                                                           float target = 0.0f,
                                                           float compliance = 0.0f,
                                                           float damping = 0.0f,
                                                           float minTwist = 0.0f,
                                                           float maxTwist = 0.0f);

    MESHTOOLS_EXPORT static JointParams makeCylinderJoint(float compliance = 0.0f,
                                                          float distance = 0.0f,
                                                          float minTwist = 0.0f,
                                                          float maxTwist = 0.0f);

    MESHTOOLS_EXPORT static JointParams makeHingeJoint(bool hasLimits = false,
                                                       float minAngle = -MaxFloat,
                                                       float maxAngle = MaxFloat,
                                                       bool hasTarget = false,
                                                       float targetAngle = 0.0f,
                                                       float compliance = 0.0f,
                                                       float damping = 0.0f);

    MESHTOOLS_EXPORT static JointParams makeServoJoint(float compliance = 0.0f, float angle = 0.0f);

    MESHTOOLS_EXPORT static JointParams makeMotorJoint(float compliance = 0.0f,
                                                       float angle = 0.0f,
                                                       float angularVelocity = 0.0f,
                                                       float minVelocity = -MaxFloat,
                                                       float maxVelocity = MaxFloat);


    MESHTOOLS_EXPORT static JointParams makeBallJoint(bool hasLimits = false,
                                                      float maxSwing = MaxFloat,
                                                      float minTwist = -MaxFloat,
                                                      float maxTwist = MaxFloat,
                                                      bool hasTarget = false,
                                                      const Vec3 targetAnges = Vec3(0.0f, 0.0f, 0.0f),
                                                      float compliance = 0.0f,
                                                      float damping = 0.0f);

    MESHTOOLS_EXPORT static JointParams makeDistanceJoint(float restLength, float compliance);

    MESHTOOLS_EXPORT static JointParams makeFixedJoint(float compliance = 0.0f);
};


/** @brief Simulation parameters.
// @param timeStep The time step of the simulation.
// @param numSubsteps The number of substeps to use for the simulation.
// @param numIterations The number of solver iterations per time step.
// @param numJointIterations The number of iterations per substep to solve joint constraints
// @param ignoreOverlappingBodyCollisions If true, bodies overlapping in the rest state are ignored during collision
detection.
// @param gravity The gravity vector. This is used to simulate gravity in the scene.
// @param contactBand The contact band. This is used to determine the distance at which two bodies are considered to be
in contact.
// @param hardConstraintCompliance The compliance of hard constraints like joint attachments or collisions.
Making this compliance larger than 0.0 results in a more stable simulation, but also in a less accurate softer
simulation.
// @param dragCompliance The compliance of drag spring.
*/
struct SimulationParams
{
    float timeStep = 1.0f / 60.0f; // The time step of the simulation.
    int numSubsteps = 5;
    int numIterations = 1;
    int numJointIterations = 2;
    bool ignoreOverlappingBodyCollisions = false;
    Vec3 gravity = Vec3(0.0f, -9.81f, 0.0f);
    float contactBand = 0.02f;
    float hardConstraintCompliance = 0.0f;
    float dragCompliance = 0.01f;
    Plane groundPlane = Plane(Vec3(0.0f, 1.0f, 0.0f), 0.0f);
};

/** @brief Describes the intersection of a ray with an object in the scene.
 * objNr The index of the object that was hit.
 * faceNr = The index of the face where the object is hit. Minus one for objects without meshes.
 * t The distance along the ray where the hit occurred.
 * normal The normal on the object at the hit point.
 */
struct RaycastHit
{
    int objectNr = -1;
    int faceNr = -1;
    float t = 0.0f;
    Vec3 normal = Vec3(Zero);
};

/**
 * @brief Parameters for creating debug lines.
 * @param getBodyMeshes If true, the meshes of the bodies are returned.
 * @param getBodyDepths If true, the depths of the bodies are returned.
 * @param getBodyFrames If true, the frames of the bodies are returned.
 * @param getJoints If true, the joints are returned.
 * @param getContacts If true, the contacts are returned.
 * @param getObjectBounds If true, the bounds of the objects are returned.
 */

struct SimulationDebugParameters
{
    bool getBodyMeshes = false;
    bool getBodyDepths = false;
    bool getBodyFrames = false;
    bool getJoints = false;
    bool getContacts = false;
    bool getObjectBounds = false;
};


/**
 * @brief The simulator
 */

class Simulator
{

public:
    /// Constructs an empty simulator
    MESHTOOLS_EXPORT Simulator();
    MESHTOOLS_EXPORT ~Simulator();

    /**
     * @brief Clear the contents of the simulator.
     */
    MESHTOOLS_EXPORT void clear();

    /**
     * @brief Interpret the passed mesh as a rigid body and add it to the simulation.
     * @param params The parameters of the rigid body.
     * @param pose The pose of the rigid body in global coordinates.
     * @param vertices The vertices of the mesh.
     * @param indices The indices of the mesh.
     * @param faceSizes The sizes of the faces of the mesh.
     * @return The index of the rigid body.
     */
    MESHTOOLS_EXPORT int addRigidMesh(const RigidBodyParams& params,
                                      const Transform& pose,
                                      const StaticBuffer<Vec3>& vertices,
                                      const StaticBuffer<int>& indices,
                                      const StaticBuffer<int>& faceSizes);

    /**
     * @brief Add a capsule as a rigid body to the simulation.
     * @param params The parameters of the rigid body.
     * @param pose The pose of the rigid body in global coordinates.
     * @param radius The radius of the capsule.
     * @param length The length of the capsule. The capsule is aligned along the x-axis.
     */
    MESHTOOLS_EXPORT int addRigidCapsule(const RigidBodyParams& params, const Transform& pose, float radius, float length);

    /**
     * @brief Add a capsule as a rigid body to the simulation defined by two points and a radius.
     * @param params The parameters of the rigid body.
     * @param p1 The first point of the capsule.
     * @param p2 The second point of the capsule.
     * @param radius The radius of the capsule. The capsule is aligned along the line between p1 and p2.
     */
    MESHTOOLS_EXPORT int addRigidCapsule(const RigidBodyParams& params, const Vec3& p1, const Vec3& p2, float radius);

    /**
     * @brief Add a joint between two bodies. If bodyNr1 is -1, the joint is connected to the world.
     * @param bodyId0 The index of the first body.
     * @param bodyId1 The index of the second body. If this is -1, the joint is connected to the world.
     * @param globalFrame The frame of the joint in global coordinates at creation time.
     * @param damping The damping of the joint.
     * @return The index of the joint.
     */
    MESHTOOLS_EXPORT int addJoint(int bodyId0, int bodyId1, const Transform& globalFrame, const JointParams& params);

    /**
     * @brief Call this function after the complete scene is set up.
     */
    MESHTOOLS_EXPORT void finalize();

    MESHTOOLS_EXPORT void setSimulationParams(const SimulationParams& params) const;

    MESHTOOLS_EXPORT void getSimulationParams(SimulationParams& params) const;

    /**
     * @brief Set the target position or angle of a joint depending on the joint type.
     * @param jointId The index of the joint.
     * @param target The target position or angle.
     */
    MESHTOOLS_EXPORT void setJointTargetAngle(int jointId, float targetAngle);

    /**
     * @brief Set the target position or angle of a joint depending on the joint type.
     * @param jointId The index of the joint.
     * @param targetDistance The target distance of the joint.
     */
    MESHTOOLS_EXPORT void setJointTargetDistance(int jointId, float targetDistance);

    /**
     * @brief Set the velocity of a joint.
     * @param jointId The index of the joint.
     * @param velocity The velocity of the joint.
     */
    MESHTOOLS_EXPORT void setJointVelocity(int jointId, float velocity);

    /**
     * @brief Set the target pose of a rigid body. This is treated as a constraint.
     * The pose of the body is not changed directly.
     * @param bodyNr The index of the body.
     * @param pose The target pose of the body.
     * @param compliance The compliance of the constraint.
     */
    MESHTOOLS_EXPORT void setRigidBodyTargetPose(int bodyNr, const Transform& pose, float compliance = 0.0f);

    /**
     * @brief Set the pose of a rigid body.
     * The pose of the body is changed directly.
     * @param bodyNr The index of the body.
     * @param pose The pose of the body.
     */
    MESHTOOLS_EXPORT void setRigidBodyPose(int bodyNr, const Transform& pose);

    /**
     * @brief Removes the target pose constraint of a rigid body.
     * @param bodyNr The index of the body.
     */
    MESHTOOLS_EXPORT void clearRigidBodyTargetPose(int bodyNr);

    /**
     * @brief Runs the simulation for one time step.
     */
    MESHTOOLS_EXPORT void stepSimulation();

    /**
     * @brief Get the pose of a rigid body.
     * @param bodyNr The index of the body.
     * @param pose The transform of the principal axes of the body.
     * @return True if the body exists.
     */
    MESHTOOLS_EXPORT bool getRigidBodyPose(int bodyId, Transform& pose) const;

    /**
     * @brief Get the transform of a rigid body.
     * @param bodyId The index of the body.
     * @param transform The transform of the body
     * @return True if the body exists.
     */
    MESHTOOLS_EXPORT bool getRigidBodyTransform(int bodyId, Transform& transform) const;

    /**
     * @brief Get the parameters of a joint.
     * @param jointId The index of the joint.
     * @param params The joint parameters.
     * @return True if the joint exists.
     */
    MESHTOOLS_EXPORT bool getJointParams(int jointId, JointParams& params) const;

    /**
     * @brief Get the bodies connected by a joint.
     * @param jointId The index of the joint.
     * @param bodyId0 The index of the first body.
     * @param bodyId1 The index of the second body.
     * @return True if the joint exists.
     */
    MESHTOOLS_EXPORT bool getJointBodies(int jointId, int& bodyId0, int& bodyId1) const;

    /**
     * @brief Get the number of rigid bodies.
     * @return The number of rigid bodies.
     */
    MESHTOOLS_EXPORT int getNumRigidBodies() const;

    /**
     * @brief Get the number of joints.
     * @return The number of joints.
     */
    MESHTOOLS_EXPORT int getNumJoints() const;

    /**
     * @brief Casts a ray into the scene checking for intersections with objects.
     * @param ray The ray to cast.
     * @param hits The hits of the ray with the objects in the scene sorted by distance
     * @return True if the ray hit at least one object.
     */
    MESHTOOLS_EXPORT bool raycast(const Ray& ray, std::vector<RaycastHit>& hits) const;

    /**
     * @brief Returns the indices of the rigid bodies overlapping given bounds.
     * @param bounds The bounds to check for overlapping bodies.
     * @param bodyIds The indices of the bodies overlapping the bounds.
     * @return True if at least one body was found.
     */
    MESHTOOLS_EXPORT bool getOverlappingRigidBodies(const Bounds3& bounds, std::vector<int>& bodyIds) const;
    /**
     * @brief Starts dragging objects in the scene.
     * @param ray The mouse ray from the mouse position along the view direction.
     * @return True if an object was hit by the ray.
     */
    MESHTOOLS_EXPORT bool startDrag(const Ray& ray);

    /**
     * @brief Drags the object in the scene.
     * @param ray The mouse ray from the mouse position along the view direction.
     * @return True if an object was picked up by the call to startDrag.
     */
    MESHTOOLS_EXPORT bool drag(const Ray& ray);

    /**
     * @brief Stops dragging objects in the scene.
     */
    MESHTOOLS_EXPORT void stopDrag();

    /**
     * @brief Retrieves debug lines for visualization.
     * @param params The parameters. Define which debug information to retrieve.
     * @param lines The lines to draw. Two consecutive points define a line segment.
     * @param colors The colors of the lines. The size of the array matches the size of the lines array.
     */
    MESHTOOLS_EXPORT void getDebugLines(const SimulationDebugParameters& params, Vec3Array& lines, Vec3Array& colors) const;

    /**
     * @brief Retrieves debug triangles for visualization.
     * @param params The parameters. Define which debug information to retrieve.
     * @param vertices The vertices to draw.
     * @param normals The normals of the triangles.
     * @param indices The indices of the triangles.
     */
    MESHTOOLS_EXPORT void getDebugTriangles(const SimulationDebugParameters& params,
                                            Vec3Array& vertices,
                                            Vec3Array& normals,
                                            Int32Array& indices) const;

    /**
     * @brief Starts profiling the simulator.
     */

    MESHTOOLS_EXPORT void startProfiling();

    /**
     * @brief Stops profiling the simulator.
     */

    MESHTOOLS_EXPORT void stopProfiling();

    /**
     * @brief Returns the profiling string.
     * @return The profiling string.
     */

    MESHTOOLS_EXPORT const char* getProfilingString() const;


    /*! \cond PRIVATE */
protected:
    class SimulatorImpl;
    std::unique_ptr<SimulatorImpl> m_impl;
    /*! \endcond */
};

};
