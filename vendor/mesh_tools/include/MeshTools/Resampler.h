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
 * @brief Resamples a mesh using a background grid.
 *
 */
class Resampler
{
public:
    /// Constructs an empty resampler
    MESHTOOLS_EXPORT Resampler();
    MESHTOOLS_EXPORT ~Resampler();

    /**
     * @brief Clears the resampled mesh.
     */
    MESHTOOLS_EXPORT void clear();

    /**
     * @brief Creates a resampled mesh.
     * @param meshes The meshes to create the resampled mesh from
     * @param spacing The spacing of the grid used for resampling
     * @param numIters The number of iterations to relax the resampled mesh
     * @param relNormalOffset The offset of the resampled mesh to the input mesh relative to the spacing
     * @param splitEmptyCellFaces If true, a surface is created between non-empty cells whose common face is empty
     * @param removeBackfaceTriangles If true, backface triangles are removed
     */

    MESHTOOLS_EXPORT bool create(const std::vector<std::shared_ptr<Mesh>>& meshes,
                                 float spacing,
                                 int numIters,
                                 float relNormalOffset = 0.4f,
                                 bool splitEmptyCellFaces = true,
                                 bool removeBackfaceTriangles = false);

    /**
     * @brief Creates a resampled mesh.
     * @param vertices The vertices of the input mesh
     * @param indices The indices of the input mesh
     * @param faceSizes The sizes of the faces of the input mesh
     * @param spacing The spacing of the grid used for resampling
     * @param numIters The number of iterations to relax the resampled mesh
     * @param relNormalOffset The offset of the resampled mesh to the input mesh relative to the spacing
     * @param splitEmptyCellFaces If true, a surface is created between non-empty cells whose common face is empty
     * @param removeBackfaceTriangles If true, backface triangles are removed
     * @param transform The transformation which is applied to the vertices before re-sampling
     */

    MESHTOOLS_EXPORT bool create(const StaticBuffer<Vec3>& vertices,
                                 const StaticBuffer<int>& indices,
                                 const StaticBuffer<int>& faceSizes,
                                 float spacing,
                                 int numIters,
                                 float relNormalOffset = 0.4f,
                                 bool splitEmptyCellFaces = true,
                                 bool removeBackfaceTriangles = false,
                                 const Mat44& transform = Mat44(Identity));

    /**
     * @brief Creates a prism mesh by extruding projection of the input vertices along a given dimension.
     * @param vertices The vertices to be extruded
     * @param indices The indices of the input mesh
     * @param faceSizes The sizes of the faces of the input mesh
     * @param relNormalOffset The offset of the resampled mesh to the input mesh relative to the spacing
     * @param extrusionDimension The dimension along which to extrude (0 = x, 1 = y, 2 = z)
     * @param spacing The distance between the original and extruded vertices
     * @param transform The transformation which is applied to the vertices after extrusion
     */

    MESHTOOLS_EXPORT bool createPrism(const StaticBuffer<Vec3>& vertices,
                                      const StaticBuffer<int>& indices,
                                      const StaticBuffer<int>& faceSizes,
                                      float relNormalOffset,
                                      int extrusionDimension,
                                      float spacing,
                                      const Mat44& transform = Mat44(Identity));

    /**
     * @brief Returns the vertices of the resampled mesh.
     */
    MESHTOOLS_EXPORT const std::vector<Vec3>& getVertices() const;

    /**
     * @brief Returns the indices of the triangles of the resampled mesh.
     */
    MESHTOOLS_EXPORT const std::vector<int>& getTriIndices() const;

    /**
     * @brief Returns the normals of the resampled mesh.
     */
    MESHTOOLS_EXPORT const std::vector<Vec3>& getNormals() const;


    /*! \cond PRIVATE */
protected:
    class ResamplerImpl;
    MESHTOOLS_EXPORT Resampler(const ResamplerImpl&);
    std::unique_ptr<ResamplerImpl> m_impl;
    /*! \endcond */
};

}
