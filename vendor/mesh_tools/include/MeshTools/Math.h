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
#include "Vec.h"

namespace MeshTools
{

/**
 * @brief Eigenvalue decomposition of a symmetric 3x3 matrix.
 * @param A Input matrix.
 * @param eigenVectors Output matrix of eigenvectors.
 * @param eigenValues Output vector of eigenvalues.
 * @param sorted If true, the eigenvalues are sorted in descending order.
 */
MESHTOOLS_EXPORT void eigenDecomposition(const Mat33& A, Mat33& eigenVectors, Vec3& eigenValues, bool sorted = true);

/**
 * @brief Polar decomposition of a 3x3 matrix such that A = U D U^T R.
 * This function can be used to extract the rotational part of a transformation matrix as used in shape matching.
 * @param A Input matrix.
 * @param R Output the rotation matrix.
 * @param U Output the stretch tensor.
 * @param D Output the diagonal matrix of singular values.
 */
MESHTOOLS_EXPORT void polarDecomposition(const Mat33& A, Mat33& R, Mat33& U, Mat33& D);

/**
 * @brief Compute the affine transformation to transform an oriented bounding box (OBB) to another OBB.
 * @param startObb The start oriented bounding box.
 * @param targetObb The target oriented bounding box.
 * @param transform The output affine transformation.
 */
MESHTOOLS_EXPORT void getObbToObbTransform(const OBB& startObb, const OBB& targetObb, AffineTransform& transform);

} // namespace MeshTools
