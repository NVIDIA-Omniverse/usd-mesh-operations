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

#include <string>
#include <vector>

namespace MeshTools
{

inline const std::vector<std::string> characterBoneNames = { "Root",
                                                             "Pelvis",
                                                             "L_UpLeg",
                                                             "L_LoLeg",
                                                             "L_Ankle",
                                                             "L_Ball",
                                                             "R_UpLeg",
                                                             "R_LoLeg",
                                                             "R_Ankle",
                                                             "R_Ball",
                                                             "Spine1",
                                                             "Spine2",
                                                             "Spine3",
                                                             "Chest",
                                                             "Neck1",
                                                             "Neck2",
                                                             "Head",
                                                             "Clavicle",
                                                             "L_UpArm",
                                                             "L_LoArm",
                                                             "L_Wrist1",
                                                             "L_ThumbFinger1",
                                                             "L_ThumbFinger2",
                                                             "L_ThumbFinger3",
                                                             "L_Wrist2",
                                                             "L_IndexFinger1",
                                                             "L_IndexFinger2",
                                                             "L_IndexFinger3",
                                                             "L_Wrist3",
                                                             "L_MiddleFinger1",
                                                             "L_MiddleFinger2",
                                                             "L_MiddleFinger3",
                                                             "L_Wrist4",
                                                             "L_RingFinger1",
                                                             "L_RingFinger2",
                                                             "L_RingFinger3",
                                                             "L_Wrist5",
                                                             "L_PinkyFinger1",
                                                             "L_PinkyFinger2",
                                                             "L_PinkyFinger3",
                                                             "R_UpArm",
                                                             "R_LoArm",
                                                             "R_Wrist1",
                                                             "R_ThumbFinger1",
                                                             "R_ThumbFinger2",
                                                             "R_ThumbFinger3",
                                                             "R_Wrist2",
                                                             "R_IndexFinger1",
                                                             "R_IndexFinger2",
                                                             "R_IndexFinger3",
                                                             "R_Wrist3",
                                                             "R_MiddleFinger1",
                                                             "R_MiddleFinger2",
                                                             "R_MiddleFinger3",
                                                             "R_Wrist4",
                                                             "R_RingFinger1",
                                                             "R_RingFinger2",
                                                             "R_RingFinger3",
                                                             "R_Wrist5",
                                                             "R_PinkyFinger1",
                                                             "R_PinkyFinger2",
                                                             "R_PinkyFinger3" };

class CharacterSimulator
{
public:
    /**
     * @brief Creates a new character simulator.
     */
    MESHTOOLS_EXPORT CharacterSimulator();
    MESHTOOLS_EXPORT ~CharacterSimulator();

    /**
     * @brief Clears the character simulator.
     */
    MESHTOOLS_EXPORT void clear();

    /**
     * @brief Adds a mesh the character will collide with.
     * @param vertices The vertices of the collision mesh.
     * @param indices The indices of the collision mesh.
     * @return The index of the added collision mesh.
     */
    MESHTOOLS_EXPORT int addCollisionMesh(const StaticBuffer<Vec3>& vertices, const StaticBuffer<int>& indices);

    /**
     * @brief Adds the stage the character will collide with.
     * @param stage The stage to add collision meshes from.
     */
    MESHTOOLS_EXPORT void addCollisionMeshes(const Stage& stage);

    /**
     * @brief Adds a character to the simulator.
     * @param rootPose The root pose of the character.
     * @param size The size of the character.
     * @return The index of the added character.
     */
    MESHTOOLS_EXPORT int addCharacter(const Transform rootPose, float size = 1.8f);

    /**
     * @brief Sets the target pose for a bone. The simulator will try to reach this pose.
     * @param characterNr The index of the character.
     * @param type The type of the bone.
     * @param targetPose The target pose for the bone.
     * @return True if the target pose was set.
     */
    MESHTOOLS_EXPORT bool setBoneTargetPose(int characterNr, const std::string& name, const Transform& targetPose);

    /**
     * @brief Steps the simulation forward by a time step.
     * @param dt The time step for the simulation.
     * @param gravity The gravity vector affecting the simulation.
     */
    MESHTOOLS_EXPORT void stepSimulation(float dt, const Vec3& gravity);

    /**
     * @brief Gets the current pose of a bone.
     * @param characterNr The index of the character.
     * @param boneNr The index of the bone.
     * @param pose The current pose of the bone.
     * @return True if the pose was retrieved.
     */
    MESHTOOLS_EXPORT bool getBonePose(int characterNr, const std::string& name, Transform& pose) const;

    /**
     * @brief Gets the number of characters in the simulator.
     * @return The number of characters in the simulator.
     */
    MESHTOOLS_EXPORT int getCharacterCount() const;

    /**
     * @brief Retrieves debug lines for visualization.
     * @param lines The lines to draw. Two consecutive points define a line segment.
     * @param colors The colors of the lines. The size of the array matches the size of the lines array.
     */
    MESHTOOLS_EXPORT void getDebugLines(Vec3Array& lines, Vec3Array& colors) const;

    /**
     * @brief Retrieves debug triangles for visualization.
     * @param vertices The vertices to draw.
     * @param normals The normals of the triangles.
     * @param indices The indices of the triangles.
     * @param colors The colors of the lines. The size of the array matches the size of the lines array.
     */
    MESHTOOLS_EXPORT void getDebugTriangles(Vec3Array& vertices, Vec3Array& normals, Int32Array& indices) const;

    /**
     * @brief Casts a ray checking for intersections bones.
     * @param ray The ray to cast.
     * @param characterNr The index of the character that was hit.
     * @param boneNr The index of the bone that was hit.
     * @param t The distance along the ray to the intersection point.
     * @return True if the ray hit a bone.
     */
    MESHTOOLS_EXPORT bool raycast(const Ray& ray, int& characterNr, int& boneNr, float& t) const;

    /**
     * @brief Starts dragging a character character if it was hit by a ray.
     * @param ray The mouse ray from the mouse position along the view direction.
     * @return True if a character was hit by the ray.
     */
    MESHTOOLS_EXPORT bool startDrag(const Ray& ray);

    /**
     * @brief Drags the character.
     * @param ray The mouse ray from the mouse position along the view direction.
     * @return True if the character was picked up by the call to startDrag.
     */
    MESHTOOLS_EXPORT bool drag(const Ray& ray);

    /**
     * @brief Stops dragging the character.
     */
    MESHTOOLS_EXPORT void stopDrag();

    /*! \cond PRIVATE */
protected:
    class CharacterSimulatorImpl;
    std::unique_ptr<CharacterSimulatorImpl> m_impl;
    /*! \endcond */
};


}
