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
 * @brief Decimates a triangle mesh.
 *
 */
class Decimator
{
public:
    /**
     * @brief Creates a new decimator.
     */
    MESHTOOLS_EXPORT Decimator();
    MESHTOOLS_EXPORT ~Decimator();

    /**
     * @brief Initializes the decimator with the given mesh.
     * @param mesh The mesh to decimate.
     * @param preserveBorder If true, the border of the mesh will not be decimated.
     * @param useGpu If true a simpified decimation method using the GPU is used
     * @return True if the decimator was successfully initialized, false otherwise.
     */
    MESHTOOLS_EXPORT bool initialize(const Mesh& mesh, bool preserveBorder = false, bool useGpu = false);

    /**
     * @brief Performs a single decimation step by collapsing a single edge.
     * @return True if an edge was collapsed, false otherwise.
     */
    MESHTOOLS_EXPORT void step();

    /**
     * @brief Decimates the mesh by the given ratio.
     * @param ratio The number of triangles to keep. A value of 0.1 will reduce the triangle count to 10% of its
     * original size.
     */
    MESHTOOLS_EXPORT void decimateByRatio(float ratio = 0.5f);

    /**
     * @brief Decimates the mesh to the given target triangle count.
     * @param targetTriCount The number of triangles to keep.
     */
    MESHTOOLS_EXPORT void decimateBySize(int targetTriCount = 100);

    /**
     * @brief Returns the decimated mesh. The decimator always returns a triangle mesh.
     * Only the vertex positions, indices, face sizes and array attributes of the provided mesh are changed.
     * Value properties and other mesh properties are not changed.
     * @param mesh The decimated mesh.
     */
    MESHTOOLS_EXPORT void getDecimatedMesh(Mesh& mesh);

    /**
     * @brief Returns debug lines for visualization of the decimation process.
     * The buffers can be used as they are in a glDrawArrays(GL_LINES, ..) call.
     * @param lines The debug lines as segments defined by pairs of vertices.
     * @param colors One color value for each vertex.
     */
    MESHTOOLS_EXPORT void getDebugLines(std::vector<Vec3>& lines, std::vector<Vec3>& colors) const;

    /*! \cond PRIVATE */
protected:
    class DecimatorImpl;
    MESHTOOLS_EXPORT Decimator(const DecimatorImpl&);
    std::unique_ptr<DecimatorImpl> m_impl;
    /*! \endcond */
};

}
