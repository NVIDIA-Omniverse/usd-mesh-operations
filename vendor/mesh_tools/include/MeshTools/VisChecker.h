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

#include <memory>
#include <string>
#include <vector>

namespace MeshTools
{

/**
 * @brief Parameters for the visibility checker.
 */
struct VisCheckerParams
{
    /**
     * @brief Whether to cluster the stage before checking visibility.
     * Clustering first splits the stage into clusters of meshes with overlapping bounding boxes,
     * then checks the visibility of the meshes in each cluster separately.
     * This allows for a more fine-grained visibility test
     */
    bool clustered = true;

    /**
     * @brief The minimum gap size.
     * This corresponds to the spacing of the background grid.
     * The spacing is clamped if the resulting number of cells in the background grid exceeds a threshold.
     * This restriction is potentially reduced by using a grid per cluster instead of a single grid for the whole stage.
     */
    float minimumGapSize = 0.01f;

    /**
     * @brief The maximum grid resolution.
     * This is the maximum number of cells along the longest axis of the grid used for visibility checking.
     * The spacing of the grid defines the maximal gap considered as closed.
     * A value of 500 is suitable for powerful GPUs.
     * For less powerful GPUs or CPUs, a smaller value should be used.
     */
    float maximumGridResolution = 500.0f;

    /**
     * @brief The granularity of the visibility check.
     * This determines the level of detail of the visibility check.
     * FACE is the most detailed, followed by ISLAND and MESH.
     */
    Granularity granularity = Granularity::ISLAND;

    /**
     * @brief Whether to create lines for debug visualization.
     */
    bool createLines = false;

    /**
     * @brief The verbosity level of the output.
     */
    int verbosityLevel = 0;

    /**
     * @brief Use multithreading to parallelize hot phases of the CPU visibility checker.
     * When false (default), behavior matches the original sequential implementation.
     * Debug line output (@ref createLines) forces a sequential path for vertex/triangle passes.
     */
    bool useParallelCpuVisChecker = false;
};

/**
 * @brief Checks on the GPU whether meshes, islands or faces are visible in a given stage.
 * The visibility of each mesh can be queried using the isVisible() function in the meshes.
 * The visibility of individual faces can be queried using the getFaceVisible() function in the meshes.
 */

class VisChecker
{

public:
    /// Constructs an empty resampler
    VisChecker(){};
    ~VisChecker(){};


    /**
     * @brief Determines the visibility of the meshes in the stage.
     * @param stage The stage to consider
     * @param params The parameters for the visibility check. See VisCheckerParams for more details.
     * @return true if the visibility check was successful.
     * The visibility of each mesh can be queried using the isVisible() function in the meshes.
     * The visibility of individual faces can be queried using the getFaceVisible() function in the meshes.
     */

    virtual bool check(Stage& stage, const VisCheckerParams& params) = 0;

    /**
     * @brief Legacy function with explicit parameters and without clustering for backward compatibility.
     * This function is deprecated. Use check(Stage& stage, const VisCheckerParams& params) instead.
     */
    virtual bool check(Stage& stage,
                       float gridResolution,
                       Granularity granularity = Granularity::ISLAND,
                       bool createLines = false,
                       int verbosityLevel = 0) = 0;

    /**
     * @brief Returns the properties of the grid used for visibility checks.
     */
    virtual void getGridDimensions(int& numX, int& numY, int& numZ, Vec3& origin, float& spacing) const = 0;

    /**
     * @brief Returns true if a cell in the grid is an air cell.
     */
    virtual bool isAirCell(int x, int y, int z) const = 0;

    /**
     * @brief Returns true if a cell in the grid is intersected by a mesh.
     */
    virtual bool isGeomCell(int x, int y, int z) const = 0;

    /**
     * @brief Returns the spacing of the grid used for visibility checks.
     */
    virtual float getSpacing() const = 0;

    /**
     * @brief Returns lines from vertices to aircells that render faces visible.
     * Set createLines to true in check() to get the lines.
     */
    virtual const std::vector<Vec3>& getVertLines() const = 0;

    /**
     * @brief Returns lines from samples on triangles to aircells that render faces visible.
     * Set createLines to true in check() to get the lines.
     */
    virtual const std::vector<Vec3>& getTriLines() const = 0;

    /**
     * @brief Sets a callback function to get logging information.
     * @param function This is called when any progress is made.
     * @see LogCallbackFunction in Defines.h
     * @param userData The user data passed to the callback function.
     */
    virtual void setLogCallbackFunction(LogCallbackFunction /*function*/, void* /*userData*/ = nullptr)
    {
    }
};

}
