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
#include "Mesh.h"
#include "Vec.h"

#include <memory>
#include <string>
#include <vector>


namespace MeshTools
{

/**
 * @brief Remeshes a triangle mesh.
 *
 */
class Remesher
{
public:
    /**
     * @brief Creates a new remesher.
     */
    MESHTOOLS_EXPORT Remesher();
    MESHTOOLS_EXPORT ~Remesher();

    /**
     * @brief Initializes the remeser with the given mesh.
     * @param mesh The mesh to remesh.
     * @param targetEdgeLength The goal of the remeser is to make the length of all edges as close as possible to this
     * value.
     * @param creaseThreshold Defines a sharpness threshold between 0.0 and 1.0 for an edge to be considered a crease
     * and therefore preserved. Increasing the value increases the number of creases.
     * @param curvedProjection Use a curved interpolation of the input mesh for vertex projection.
     * This yields a smoother mesh for low resolution input meshes.
     * @param preserveBorder If true, the border of the mesh will not be remeshed.
     * @param mergeVertices If true, points that are close to each other will be merged.
     * @return True if the remesher was successfully initialized, false otherwise.
     */
    MESHTOOLS_EXPORT bool initialize(const Mesh& mesh,
                                     float targetEdgeLength = 0.2f,
                                     float creaseThreshold = 0.6f,
                                     bool preserveBorder = false,
                                     bool mergeVertices = false);

    /**
     * @brief Performs a series of remeshing steps. A step consists of moving vertices and flipping, splitting and
     * collapsing edges.
     * @param numberOfSteps The number of steps to perform.
     * @return True if the remeshing was successful, false otherwise.
     */
    MESHTOOLS_EXPORT void remesh(int numberOfSteps = 3);

    /**
     * @brief Projects the new mesh to the input mesh using curved interpolation to get a smoother output
     */
    MESHTOOLS_EXPORT void projectToCurvedSurface();

    /**
     * @brief Returns the remeshed mesh. The remesher always returns a triangle mesh.
     * Only the vertex positions, indices, face sizes and array attributes of the provided mesh are changed.
     * Value properties and other mesh properties are not changed.
     * @param mesh The remeshed mesh.
     */
    MESHTOOLS_EXPORT void getRemeshedMesh(Mesh& mesh);

    /**
     * @brief Returns debug lines for visualization of the remeshing process.
     * The buffers can be used as they are in a glDrawArrays(GL_LINES, ..) call.
     * @param lines The debug lines as segments defined by pairs of vertices.
     * @param colors One color value for each vertex.
     */
    MESHTOOLS_EXPORT void getDebugLines(std::vector<Vec3>& lines, std::vector<Vec3>& colors) const;

    /*! \cond PRIVATE */
protected:
    class RemesherImpl;
    MESHTOOLS_EXPORT Remesher(const RemesherImpl&);
    std::unique_ptr<RemesherImpl> m_impl;
    /*! \endcond */
};

}
