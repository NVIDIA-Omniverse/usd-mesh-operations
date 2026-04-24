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
#include "VisChecker.h"

#include <memory>
#include <string>
#include <vector>

namespace MeshTools
{

/**
 * @brief Checks on the CPU whether meshes, islands or faces are visible in a given stage.
 *
 */

class VisCheckerCPU : public VisChecker
{
public:
    /// Constructs an empty resampler
    MESHTOOLS_EXPORT VisCheckerCPU();
    MESHTOOLS_EXPORT ~VisCheckerCPU();

    /**
     * @brief Determines the visibility of the meshes in the stage.
     * @param stage The stage to consider
     * @param params The parameters for the visibility check. See VisCheckerParams for more details.
     * @return true if the visibility check was successful.
     * The visibility of each mesh can be queried using the isVisible() function in the meshes.
     * The visibility of individual faces can be queried using the getFaceVisible() function in the meshes.
     */
    MESHTOOLS_EXPORT bool check(Stage& stage, const VisCheckerParams& params) override;

    /**
     * @brief Legacy function with explicit parameters and without clustering for backward compatibility.
     * This function is deprecated. Use check(Stage& stage, const VisCheckerParams& params) instead.
     */
    MESHTOOLS_EXPORT bool check(Stage& stage,
                                float gridResolution,
                                Granularity granularity = Granularity::ISLAND,
                                bool createLines = false,
                                int verbosityLevel = 0) override;

    /**
     * @brief Returns the properties of the grid used for visibility checks.
     */
    MESHTOOLS_EXPORT void getGridDimensions(int& numX, int& numY, int& numZ, Vec3& origin, float& spacing) const override;

    /**
     * @brief Returns true if a cell in the grid is an air cell.
     */
    MESHTOOLS_EXPORT bool isAirCell(int x, int y, int z) const override;

    /**
     * @brief Returns true if a cell in the grid is intersected by a mesh.
     */
    MESHTOOLS_EXPORT bool isGeomCell(int x, int y, int z) const override;

    /**
     * @brief Returns the spacing of the grid used for visibility checks.
     */
    MESHTOOLS_EXPORT float getSpacing() const override;

    /**
     * @brief Returns lines from vertices to aircells that render faces visible.
     * Set createLines to true in check() to get the lines.
     */
    MESHTOOLS_EXPORT const std::vector<Vec3>& getVertLines() const override;

    /**
     * @brief Returns lines from samples on triangles to aircells that render faces visible.
     * Set createLines to true in check() to get the lines.
     */
    MESHTOOLS_EXPORT const std::vector<Vec3>& getTriLines() const;

    /**
     * @brief Sets a callback function to receive logging information during visibility checks.
     * @param function Callback invoked with log messages (nullptr to disable).
     * @param userData User data passed to the callback.
     */
    MESHTOOLS_EXPORT void setLogCallbackFunction(LogCallbackFunction function, void* userData = nullptr) override;

    /*! \cond PRIVATE */
protected:
    class VisCheckerCPUImpl;
    MESHTOOLS_EXPORT VisCheckerCPU(const VisCheckerCPUImpl&);
    std::unique_ptr<VisCheckerCPUImpl> m_impl;
    /*! \endcond */
};

}
