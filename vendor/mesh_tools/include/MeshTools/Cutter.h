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
 * @brief Cutts a mesh.
 *
 */
class Cutter
{
public:
    /**
     * @brief Creates a cutter
     */
    MESHTOOLS_EXPORT Cutter();
    MESHTOOLS_EXPORT ~Cutter();

    /**
     * @brief Initializes the cutter with the given mesh.
     * @param mesh The mesh to decimate.
     * @return True if the cutter was successfully initialized, false otherwise.
     */
    MESHTOOLS_EXPORT bool initialize(const Mesh& mesh);

    /**
     * @brief Cuts the mesh along a plane.
     * @param plane The plane to cut the mesh along.
     * @return True if the mesh was successfully cut, false otherwise.
     */
    MESHTOOLS_EXPORT bool planeCut(const Plane& plane);
    /**
     * @brief Cuts the mesh along a backgroud grid.
     * @param cellSize The size of the grid cells. The spacing can be different in each dimension.
     * @param gridRotation The rotation of the grid for non-axis aligned cuts.
     * @return True if the mesh was successfully cut, false otherwise.
     */
    MESHTOOLS_EXPORT bool gridCut(const Vec3& cellSize, const Quat& gridRotation);

    /**
     * @brief Cuts the along a second mesh which is used as a stencil.
     * @return True if the mesh was successfully cut, false otherwise.
     */
    MESHTOOLS_EXPORT bool meshCut(const Mesh& mesh, const AffineTransform& meshTransform = AffineTransform(Identity));

    /**
     * @brief Returns the cut mesh.
     * @param mesh The decimated mesh.
     */
    MESHTOOLS_EXPORT void getCutMesh(Mesh& mesh);

    /**
     * @brief Returns debug lines for visualization of the cutting process.
     * The buffers can be used as they are in a glDrawArrays(GL_LINES, ..) call.
     * @param lines The debug lines as segments defined by pairs of vertices.
     * @param colors One color value for each vertex.
     */
    MESHTOOLS_EXPORT void getDebugLines(std::vector<Vec3>& lines, std::vector<Vec3>& colors) const;

    /*! \cond PRIVATE */
protected:
    class CutterImpl;
    MESHTOOLS_EXPORT Cutter(const CutterImpl&);
    std::unique_ptr<CutterImpl> m_impl;
    /*! \endcond */
};

}
