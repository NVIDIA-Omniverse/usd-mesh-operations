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
#include "Mesh.h"
#include "Vec.h"

#include <memory>
#include <string>
#include <vector>


namespace MeshTools
{

/**
 * @brief Trianulates a set of 2d points potentially constrained by a set of edges.
 *
 */
class Triangulator
{
public:
    /**
     * @brief Creates a new triangulator.
     */
    MESHTOOLS_EXPORT Triangulator();
    MESHTOOLS_EXPORT ~Triangulator();

    /**
     * @brief Creates a triangulation of the given points which respects the given edges.
     * @param points The 2d points to triangulate.
     * @param edgeIndices The indices of the points which form the edges. Each edge is defined by
     * @return True if the triangulation was successful.
     * two consecutive indices. The edges are treated as hard constraints.
     */

    MESHTOOLS_EXPORT bool create(const std::vector<Vec2>& points, const std::vector<int>& edgeIndices);

    /**
     * @brief Returns the indices of the triangles which form the triangulation.
     * @return The indices of the triangles.
     */

    MESHTOOLS_EXPORT const std::vector<int>& getTriangleIndices();

    /**
     * @brief Returns debug lines for visualization of the triangulation process.
     * The buffers can be used as they are in a glDrawArrays(GL_LINES, ..) call.
     * @param lines The debug lines as segments defined by pairs of vertices.
     * @param colors One color value for each vertex.
     */
    MESHTOOLS_EXPORT void getDebugLines(std::vector<Vec3>& lines, std::vector<Vec3>& colors) const;

    /*! \cond PRIVATE */
protected:
    class TriangulatorImpl;
    MESHTOOLS_EXPORT Triangulator(const TriangulatorImpl&);
    std::unique_ptr<TriangulatorImpl> m_impl;
    /*! \endcond */
};

}
