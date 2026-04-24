// SPDX-FileCopyrightText: Copyright (c) 2023-2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
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

#include <memory>
#include <vector>

namespace MeshTools
{

/**
 * @brief Represents a curve.
 *
 */
class Curve
{
public:
    /**
     * @brief Creates a new curve.
     */
    MESHTOOLS_EXPORT Curve();

    MESHTOOLS_EXPORT ~Curve();

    /**
     * @brief Creates a curve from a set of points.
     * @param points The points that define the curve.
     * @param numPoints The number of points in the curve.
     */
    MESHTOOLS_EXPORT bool create(const Vec3* points, int numPoints);

    /**
     * @brief Returns the number of points in the curve.
     * @return The number of points in the curve.
     */
    MESHTOOLS_EXPORT size_t pointCount();

    /**
     * @brief Returns the points of the curve (in local space).
     * @return The points of the curve.
     */
    MESHTOOLS_EXPORT const Vec3* points() const;

    /**
     * @brief Sets the transform (pose) of the curve. Points are stored in local space; the transform is not applied to
     * them.
     */
    MESHTOOLS_EXPORT void setTransform(const AffineTransform& transform);

    /**
     * @brief Returns the transform of the curve.
     */
    MESHTOOLS_EXPORT const AffineTransform& transform() const;

    /**
     * @brief Returns the distance to the closest point on the curve.
     * @param position The position to compute the distance from.
     * @param closestPoint The closest point on the curve. If nullptr, the closest point is not returned.
     * @param tangent The tangent of the curve at the closest point. If nullptr, the tangent is not returned.
     * @return The distance of the curve.
     */
    MESHTOOLS_EXPORT float getClosestDistance(const Vec3& position,
                                              Vec3* closestPoint = nullptr,
                                              Vec3* tangent = nullptr) const;

    /**
     * @brief Returns the total length of the curve.
     * @return The total length of the curve.
     */
    MESHTOOLS_EXPORT float length();

    /**
     * @brief Returns the interpolated position on the curve at a given distance along the curve.
     * @param distance The distance along the path.
     * @param curved If true, the curve is interpolated with a curve of a sequence of splines.
     * @return The interpolated position on the curve.
     */
    MESHTOOLS_EXPORT Vec3 evaluatePosition(float distance, bool curved = false) const;

    /**
     * @brief Returns the tangent of the path at a given distance along the path.
     * @param distance The distance along the path.
     * @param curved If true, the curve is interpolated with a curve of a sequence of splines.
     * @return The tangent of the path.
     */
    MESHTOOLS_EXPORT Vec3 evaluateTangent(float distance, bool curved = false) const;

    /**
     * @brief Returns the curvature of the curve at a given distance along the curve.
     * @param distance The distance along the curve.
     * @param curved If true, the curve is interpolated with a curve of a sequence of splines.
     * @return The curvature of the curve.
     */
    MESHTOOLS_EXPORT float evaluateCurvature(float distance, bool curved = false) const;

    /**
     * @brief Samples the curve at roughly uniform arc-length intervals using spline interpolation.
     * @param spacing Arc-length step between samples.
     * @param points Output positions in local space (cleared and filled by this call).
     */
    MESHTOOLS_EXPORT void sample(float spacing, std::vector<Vec3>& points) const;

    /*! \cond PRIVATE */
protected:
    class CurveImpl;
    MESHTOOLS_EXPORT Curve(const CurveImpl&);
    std::unique_ptr<CurveImpl> m_impl;
    friend class NavigationController;
    /*! \endcond */
};

} // namespace MeshTools
