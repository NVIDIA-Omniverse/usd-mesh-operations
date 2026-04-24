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
#include "Stage.h"
#include "Vec.h"

#include <memory>
#include <string>
#include <vector>

namespace MeshTools
{

/**
 * @brief Finds similar meshes in a stage.
 *
 */
class DuplicationChecker
{

public:
    /// Constructs an empty duplication checker
    MESHTOOLS_EXPORT DuplicationChecker();
    MESHTOOLS_EXPORT ~DuplicationChecker();

    /**
     * @brief Assigns a duplication set number to each mesh. Meshes in the same set are similar.
     * @param stage The stage to check
     * @param relTolerance The relative tolerance to use for comparing meshes
     * @param allowScaling If true, meshes that are similar up to a scaling factor are considered similar.Otherwise, the
     * sizes of the meshes are used for the similarity tests as well.
     * @param matchVertexCount If true, meshes with different vertex counts are not considered similar.
     * @param optimizeOBBs If true, optimzes towards the volume minimizing OBB using multiple 2d projections. This
     increases the computation time.
     * @param useGPU If true, the GPU is used for the comparison
     * @return The number of duplication sets found

     */
    MESHTOOLS_EXPORT int findDuplicationSets(Stage& stage,
                                             float relTolerance = 0.01f,
                                             bool allowScaling = false,
                                             bool matchVertexCount = false,
                                             bool optimizeOBBs = false,
                                             bool useGPU = false);

    /**
     * @brief Returns the duplication sets.
     * Each set contains a list of mesh numbers
     */
    MESHTOOLS_EXPORT const std::vector<std::vector<size_t>>& duplicationSets() const;

    /*! \cond PRIVATE */
protected:
    class DuplicationCheckerImpl;
    MESHTOOLS_EXPORT DuplicationChecker(const DuplicationCheckerImpl&);
    std::unique_ptr<DuplicationCheckerImpl> m_impl;
    /*! \endcond */
};

}
