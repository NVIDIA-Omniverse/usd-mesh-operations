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

#include "Vec.h"

#include <math.h>
#include <vector>

namespace MeshTools
{

// ----------------------------------------------------------------------------
#define PiD 3.14159265358979323846264338327950
#define TwoPiD (2.0 * PiD)
#define HalfPiD (0.5 * PiD)
#define MaxDouble 1.7976931348623158e+308

CUDA_CALLABLE inline double Min(double d0, double d1)
{
    return d0 < d1 ? d0 : d1;
}

CUDA_CALLABLE inline double Max(double d0, double d1)
{
    return d0 > d1 ? d0 : d1;
}

CUDA_CALLABLE inline double Min(double d0, double d1, double d2)
{
    return Min(Min(d0, d1), d2);
}

CUDA_CALLABLE inline double Max(double d0, double d1, double d2)
{
    return Max(Max(d0, d1), d2);
}

CUDA_CALLABLE inline double Abs(double d)
{
    return d >= 0.0 ? d : -d;
}

/**
 * @brief Clamps a doubleing point value to the range [a, b].
 * @param d The value to clamp.
 * @param a The minimum value.
 * @param b The maximum value.
 * @return The clamped value.
 */
CUDA_CALLABLE inline double Clamp(double d, double a, double b)
{
    return Min(b, Max(a, d));
}

/**
 * @brief Computes the sine of an angle in radians.
 */
CUDA_CALLABLE inline double Sin(double d)
{
    return sin(d);
}

/**
 * @brief Computes the cosine of an angle in radians.
 */
CUDA_CALLABLE inline double Cos(double d)
{
    return cos(d);
}

/**
 * @brief Computes the arc sine of a value safely clamping the input to the range [-1, 1].
 */
CUDA_CALLABLE inline double Asin(double d)
{
    return asin(Clamp(d, -1.0, 1.0));
}

/**
 * @brief Computes the arc cosine of a value safely clamping the input to the range [-1, 1].
 */
CUDA_CALLABLE inline double Acos(double d)
{
    return acos(Clamp(d, -1.0, 1.0));
}

/**
 * @brief Rounds a doubleing point value to a specified number of digits.
 */
CUDA_CALLABLE inline double Round(double d, int numDigits)
{
    double p = pow(10.0, (double)numDigits);
    return d > 0.0 ? floor(p * d + 0.5) / p : ceil(p * d - 0.5) / p;
}

/**
 * @brief Defines a two dimensional vector with the double components x and y.
 */
struct Vec2d
{
    CUDA_CALLABLE Vec2d() : x(0.0), y(0.0)
    {
    }
    CUDA_CALLABLE Vec2d(_ZERO) : x(0.0), y(0.0)
    {
    }
    CUDA_CALLABLE Vec2d(double x, double y) : x(x), y(y)
    {
    }
    CUDA_CALLABLE Vec2d(_IDENTITY, int dim)
    {
        x = dim == 0 ? 1.0 : 0.0;
        y = dim == 1 ? 1.0 : 0.0;
    }
    CUDA_CALLABLE Vec2d(const Vec2& v) : x(v.x), y(v.y)
    {
    }

    CUDA_CALLABLE double& operator[](unsigned int index)
    {
        return reinterpret_cast<double*>(this)[index];
    }

    CUDA_CALLABLE const double& operator[](unsigned int index) const
    {
        return reinterpret_cast<const double*>(this)[index];
    }

    CUDA_CALLABLE bool operator==(const Vec2d& v) const
    {
        return x == v.x && y == v.y;
    }

    CUDA_CALLABLE bool operator!=(const Vec2d& v) const
    {
        return x != v.x || y != v.y;
    }

    CUDA_CALLABLE bool operator<(const Vec2d& v) const
    {
        if (x != v.x)
            return x < v.x;
        return y < v.y;
    }

    CUDA_CALLABLE bool isZero() const
    {
        return x == 0.0 && y == 0.0;
    }

    CUDA_CALLABLE void set(double newX, double newY)
    {
        x = newX;
        y = newY;
    }

    CUDA_CALLABLE Vec2d minimum(const Vec2d& v) const
    {
        return Vec2d(Min(x, v.x), Min(y, v.y));
    }

    CUDA_CALLABLE Vec2d maximum(const Vec2d& v) const
    {
        return Vec2d(Max(x, v.x), Max(y, v.y));
    }

    CUDA_CALLABLE Vec2d lerp(const Vec2d& v, double s) const
    {
        return *this * (1.0 - s) + v * s;
    }

    CUDA_CALLABLE Vec2d normal() const
    {
        return Vec2d(y, -x);
    }

    CUDA_CALLABLE Vec2d operator-() const
    {
        return Vec2d(-x, -y);
    }

    CUDA_CALLABLE Vec2d operator+(const Vec2d& v) const
    {
        return Vec2d(x + v.x, y + v.y);
    }

    CUDA_CALLABLE Vec2d operator-(const Vec2d& v) const
    {
        return Vec2d(x - v.x, y - v.y);
    }

    CUDA_CALLABLE void operator+=(const Vec2d& v)
    {
        x += v.x;
        y += v.y;
    }

    CUDA_CALLABLE void operator-=(const Vec2d& v)
    {
        x -= v.x;
        y -= v.y;
    }

    CUDA_CALLABLE void operator*=(double d)
    {
        x *= d;
        y *= d;
    }

    CUDA_CALLABLE void operator/=(double d)
    {
        x /= d;
        y /= d;
    }

    CUDA_CALLABLE Vec2d operator*(double d) const
    {
        return Vec2d(x * d, y * d);
    }

    CUDA_CALLABLE Vec2d operator/(double d) const
    {
        return Vec2d(x / d, y / d);
    }

    CUDA_CALLABLE Vec2d multiply(const Vec2d& a) const
    {
        return Vec2d(x * a.x, y * a.y);
    }

    CUDA_CALLABLE Vec2d diagonalMult(const Vec2d& v) const
    {
        return Vec2d(x * v.x, y * v.y);
    }

    CUDA_CALLABLE void setMin(const Vec2d& v)
    {
        x = x < v.x ? x : v.x;
        y = y < v.y ? y : v.y;
    }

    CUDA_CALLABLE void setMax(const Vec2d& v)
    {
        x = x > v.x ? x : v.x;
        y = y > v.y ? y : v.y;
    }

    CUDA_CALLABLE double magnitude() const
    {
        return sqrt(x * x + y * y);
    }

    CUDA_CALLABLE double magnitudeSquared() const
    {
        return x * x + y * y;
    }

    CUDA_CALLABLE double length() const
    {
        return sqrt(x * x + y * y);
    }

    CUDA_CALLABLE double lengthSquared() const
    {
        return x * x + y * y;
    }

    CUDA_CALLABLE double normalize()
    {
        double m = magnitude();
        if (m > 0.0)
            *this /= m;
        return m;
    }

    CUDA_CALLABLE Vec2d normalized() const
    {
        Vec2d v = *this;
        v.normalize();
        return v;
    }

    CUDA_CALLABLE double dot(const Vec2d& v) const
    {
        return x * v.x + y * v.y;
    }

    CUDA_CALLABLE double cross(const Vec2d& v) const
    {
        return x * v.y - y * v.x;
    }

    CUDA_CALLABLE Vec2d perp() const
    {
        return Vec2d(-y, x);
    }

    double x, y;
};

CUDA_CALLABLE inline Vec2d operator*(double d, const Vec2d& v)
{
    return Vec2d(d * v.x, d * v.y);
}

/**
 * @brief Defines a three dimensional vector with the double components x, y and z.
 */
struct Vec3d
{
    CUDA_CALLABLE Vec3d() : x(0.0), y(0.0), z(0.0)
    {
    }
    CUDA_CALLABLE Vec3d(const Vec2d& v) : x(v.x), y(v.y), z(0.0)
    {
    }
    CUDA_CALLABLE Vec3d(_ZERO) : x(0.0), y(0.0), z(0.0)
    {
    }
    CUDA_CALLABLE Vec3d(double x, double y, double z) : x(x), y(y), z(z)
    {
    }
    CUDA_CALLABLE Vec3d(_IDENTITY, int dim)
    {
        x = dim == 0 ? 1.0 : 0.0;
        y = dim == 1 ? 1.0 : 0.0;
        z = dim == 2 ? 1.0 : 0.0;
    }
    CUDA_CALLABLE Vec3d(const Vec3& v) : x(v.x), y(v.y), z(v.z)
    {
    }

    CUDA_CALLABLE double& operator[](unsigned int index)
    {
        return reinterpret_cast<double*>(this)[index];
    }

    CUDA_CALLABLE const double& operator[](unsigned int index) const
    {
        return reinterpret_cast<const double*>(this)[index];
    }

    CUDA_CALLABLE bool operator==(const Vec3d& v) const
    {
        return x == v.x && y == v.y && z == v.z;
    }

    CUDA_CALLABLE bool operator!=(const Vec3d& v) const
    {
        return x != v.x || y != v.y || z != v.z;
    }

    CUDA_CALLABLE bool operator<(const Vec3d& v) const
    {
        if (x != v.x)
            return x < v.x;
        if (y != v.y)
            return y < v.y;
        return z < v.z;
    }

    CUDA_CALLABLE bool isZero() const
    {
        return x == 0.0 && y == 0.0 && z == 0.0;
    }

    CUDA_CALLABLE void set(double newX, double newY, double newZ)
    {
        x = newX;
        y = newY;
        z = newZ;
    }

    CUDA_CALLABLE Vec3d minimum(const Vec3d& v) const
    {
        return Vec3d(Min(x, v.x), Min(y, v.y), Min(z, v.z));
    }

    CUDA_CALLABLE Vec3d maximum(const Vec3d& v) const
    {
        return Vec3d(Max(x, v.x), Max(y, v.y), Max(z, v.z));
    }

    CUDA_CALLABLE Vec3d lerp(const Vec3d& v, double s) const
    {
        return *this * (1.0 - s) + v * s;
    }

    CUDA_CALLABLE Vec3d operator-() const
    {
        return Vec3d(-x, -y, -z);
    }

    CUDA_CALLABLE Vec3d operator+(const Vec3d& v) const
    {
        return Vec3d(x + v.x, y + v.y, z + v.z);
    }

    CUDA_CALLABLE Vec3d operator-(const Vec3d& v) const
    {
        return Vec3d(x - v.x, y - v.y, z - v.z);
    }

    CUDA_CALLABLE void operator+=(const Vec3d& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
    }

    CUDA_CALLABLE void operator-=(const Vec3d& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
    }

    CUDA_CALLABLE void operator*=(double d)
    {
        x *= d;
        y *= d;
        z *= d;
    }

    CUDA_CALLABLE void operator/=(double d)
    {
        x /= d;
        y /= d;
        z /= d;
    }

    CUDA_CALLABLE Vec3d operator*(double d) const
    {
        return Vec3d(x * d, y * d, z * d);
    }

    CUDA_CALLABLE Vec3d operator/(double d) const
    {
        return Vec3d(x / d, y / d, z / d);
    }

    CUDA_CALLABLE Vec3d multiply(const Vec3d& a) const
    {
        return Vec3d(x * a.x, y * a.y, z * a.z);
    }

    CUDA_CALLABLE Vec3d diagonalMult(const Vec3d& v) const
    {
        return Vec3d(x * v.x, y * v.y, z * v.z);
    }

    CUDA_CALLABLE void setMin(const Vec3d& v)
    {
        x = x < v.x ? x : v.x;
        y = y < v.y ? y : v.y;
        z = z < v.z ? z : v.z;
    }

    CUDA_CALLABLE void setMax(const Vec3d& v)
    {
        x = x > v.x ? x : v.x;
        y = y > v.y ? y : v.y;
        z = z > v.z ? z : v.z;
    }

    CUDA_CALLABLE double magnitude() const
    {
        return sqrt(x * x + y * y + z * z);
    }

    CUDA_CALLABLE double magnitudeSquared() const
    {
        return x * x + y * y + z * z;
    }

    CUDA_CALLABLE double length() const
    {
        return sqrt(x * x + y * y + z * z);
    }

    CUDA_CALLABLE double lengthSquared() const
    {
        return x * x + y * y + z * z;
    }

    CUDA_CALLABLE double normalize()
    {
        double m = magnitude();
        if (m > 0.0)
            *this /= m;
        return m;
    }

    CUDA_CALLABLE Vec3d normalized() const
    {
        Vec3d v = *this;
        v.normalize();
        return v;
    }

    CUDA_CALLABLE double dot(const Vec3d& v) const
    {
        return x * v.x + y * v.y + z * v.z;
    }

    CUDA_CALLABLE Vec3d cross(const Vec3d& v) const
    {
        return Vec3d(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }

    CUDA_CALLABLE Vec3d perp() const
    {
        if (isZero())
            return Vec3d(1.0, 0.0, 0.0);
        Vec3d a(0.0, 0.0, 1.0);
        if (fabs(x) < fabs(y) && fabs(x) < fabs(z))
            a = Vec3d(1.0, 0.0, 0.0);
        else if (fabs(y) < fabs(z))
            a = Vec3d(0.0, 1.0, 0.0);
        Vec3d b = a.cross(*this);
        b.normalize();
        return b;
    }

    double x, y, z;
};

CUDA_CALLABLE inline Vec3d operator*(double d, const Vec3d& v)
{
    return Vec3d(d * v.x, d * v.y, d * v.z);
}

/**
 * @brief Defines a four dimensional vector with the double components x, y, z and w.
 */
struct Vec4d
{
    CUDA_CALLABLE Vec4d() : x(0.0), y(0.0), z(0.0), w(0.0)
    {
    }
    CUDA_CALLABLE Vec4d(_ZERO) : x(0.0), y(0.0), z(0.0), w(0.0)
    {
    }
    CUDA_CALLABLE Vec4d(double x, double y, double z, double w) : x(x), y(y), z(z), w(w)
    {
    }
    CUDA_CALLABLE Vec4d(const Vec3d& v, double w = 0.0) : x(v.x), y(v.y), z(v.z), w(w)
    {
    }
    CUDA_CALLABLE Vec4d(_IDENTITY, int dim)
    {
        x = dim == 0 ? 1.0 : 0.0;
        y = dim == 1 ? 1.0 : 0.0;
        z = dim == 2 ? 1.0 : 0.0;
        w = dim == 3 ? 1.0 : 0.0;
    }
    CUDA_CALLABLE Vec4d(const Vec4& v) : x(v.x), y(v.y), z(v.z), w(v.w)
    {
    }
    CUDA_CALLABLE Vec3d getXYZ() const
    {
        return Vec3d(x, y, z);
    }

    CUDA_CALLABLE double& operator[](unsigned int index)
    {
        return reinterpret_cast<double*>(this)[index];
    }

    CUDA_CALLABLE const double& operator[](unsigned int index) const
    {
        return reinterpret_cast<const double*>(this)[index];
    }

    CUDA_CALLABLE bool isZero() const
    {
        return x == 0.0 && y == 0.0 && z == 0.0 && w == 0.0;
    }

    CUDA_CALLABLE void set(double newX, double newY, double newZ, double newW)
    {
        x = newX;
        y = newY;
        z = newZ;
        w = newW;
    }

    CUDA_CALLABLE Vec4d operator-() const
    {
        return Vec4d(-x, -y, -z, -w);
    }

    CUDA_CALLABLE Vec4d operator+(const Vec4d& v) const
    {
        return Vec4d(x + v.x, y + v.y, z + v.z, w + v.w);
    }

    CUDA_CALLABLE Vec4d operator-(const Vec4d& v) const
    {
        return Vec4d(x - v.x, y - v.y, z - v.z, w - v.w);
    }

    CUDA_CALLABLE void operator+=(const Vec4d& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        w += v.w;
    }

    CUDA_CALLABLE void operator-=(const Vec4d& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        w -= v.w;
    }

    CUDA_CALLABLE void operator*=(double d)
    {
        x *= d;
        y *= d;
        z *= d;
        w *= d;
    }

    CUDA_CALLABLE void operator/=(double d)
    {
        x /= d;
        y /= d;
        z /= d;
        w /= d;
    }

    CUDA_CALLABLE Vec4d operator*(double d) const
    {
        return Vec4d(x * d, y * d, z * d, w * d);
    }

    CUDA_CALLABLE Vec4d operator/(double d) const
    {
        return Vec4d(x / d, y / d, z / d, w / d);
    }

    CUDA_CALLABLE bool operator==(const Vec4d& v) const
    {
        return x == v.x && y == v.y && z == v.z && w == v.w;
    }

    CUDA_CALLABLE bool operator!=(const Vec4d& v) const
    {
        return x != v.x || y != v.y || z != v.z || w != v.w;
    }

    CUDA_CALLABLE bool operator<(const Vec4d& v) const
    {
        if (x != v.x)
            return x < v.x;
        if (y != v.y)
            return y < v.y;
        if (z != v.z)
            return z < v.z;
        return w < v.w;
    }

    CUDA_CALLABLE void setMin(const Vec4d& v)
    {
        x = x < v.x ? x : v.x;
        y = y < v.y ? y : v.y;
        z = z < v.z ? z : v.z;
        w = w < v.w ? w : v.w;
    }

    CUDA_CALLABLE void setMax(const Vec4d& v)
    {
        x = x > v.x ? x : v.x;
        y = y > v.y ? y : v.y;
        z = z > v.z ? z : v.z;
        w = w > v.w ? w : v.w;
    }

    CUDA_CALLABLE double magnitude() const
    {
        return sqrt(x * x + y * y + z * z + w * w);
    }

    CUDA_CALLABLE double magnitudeSquared() const
    {
        return x * x + y * y + z * z + w * w;
    }

    CUDA_CALLABLE double length() const
    {
        return sqrt(x * x + y * y + z * z + w * w);
    }

    CUDA_CALLABLE double lengthSquared() const
    {
        return x * x + y * y + z * z + w * w;
    }

    CUDA_CALLABLE double normalize()
    {
        double m = magnitude();
        if (m > 0.0)
            *this /= m;
        return m;
    }

    CUDA_CALLABLE Vec4d normalized() const
    {
        Vec4d v = *this;
        v.normalize();
        return v;
    }

    CUDA_CALLABLE double dot(const Vec4d& v) const
    {
        return x * v.x + y * v.y + z * v.z + w * v.w;
    }

    double x, y, z, w;
};

CUDA_CALLABLE inline Vec4d operator*(double d, const Vec4d& v)
{
    return Vec4d(d * v.x, d * v.y, d * v.z, d * v.w);
}

/**
 * @brief Defines a plane via the equation n * p = d
 */
struct PlaneD
{
    /**
     * @var double n
     * @brief The normal of the plane.
     */

    /**
     * @var double d
     * @brief The offset of the plane.
     */

    CUDA_CALLABLE PlaneD() : n(Zero), d(0.0)
    {
    }
    CUDA_CALLABLE PlaneD(const Plane plane) : n(plane.n), d(plane.d)
    {
    }
    CUDA_CALLABLE PlaneD(const Vec3d& n, double d) : n(n), d(d)
    {
    }
    CUDA_CALLABLE PlaneD(const Vec3d& p0, const Vec3d& p1, const Vec3d& p2)
    {
        n = (p1 - p0).cross(p2 - p0);
        n.normalize();
        d = p0.dot(n);
    }

    CUDA_CALLABLE double height(const Vec3d& p) const
    {
        return p.dot(n) - d;
    }
    CUDA_CALLABLE Vec3d project(const Vec3d& p) const
    {
        return p - n * (p.dot(n) - d);
    }

    Vec3d n;
    double d;
};

/**
 * @brief Defines a ray via the equation p = orig + t * dir
 */
struct RayD
{
    /**
     * @var Vec3d orig
     * @brief The origin of the ray.
     */

    /**
     * @var Vec3d dir
     * @brief The direction of the ray.
     */

    CUDA_CALLABLE RayD() : orig(Zero), dir(Zero)
    {
    }
    CUDA_CALLABLE RayD(const Ray& ray) : orig(ray.orig), dir(ray.dir)
    {
    }
    CUDA_CALLABLE RayD(const Vec3d& orig, const Vec3d& dir) : orig(orig), dir(dir)
    {
    }

    CUDA_CALLABLE Vec3d at(double t) const
    {
        return orig + t * dir;
    }
    CUDA_CALLABLE bool intersect(const PlaneD& p, double& t)
    {
        t = dir.dot(p.n);
        if (t == 0.0)
            return false;
        t = (p.d - orig.dot(p.n)) / t;
        return true;
    }

    Vec3d orig, dir;
};

/**
 * @brief Defines a 2d ray via the equation p = orig + t * dir
 */
struct Ray2d
{
    /**
     * @var Vec2d orig
     * @brief The origin of the ray.
     */

    /**
     * @var Vec2d dir
     * @brief The direction of the ray.
     */

    CUDA_CALLABLE Ray2d() : orig(Zero), dir(Zero)
    {
    }
    CUDA_CALLABLE Ray2d(const Vec2d& orig, const Vec2d& dir) : orig(orig), dir(dir)
    {
    }

    CUDA_CALLABLE Vec2d at(double t) const
    {
        return orig + t * dir;
    }
    Vec2d orig, dir;
};

//-----------------------------------------------------------------------------

struct QuatD;

/**
 * @brief Defines a 3x3 matrix via its columns of type Vec3.
 */

struct Mat33d
{
    CUDA_CALLABLE Mat33d() : column0(Zero), column1(Zero), column2(Zero){};
    CUDA_CALLABLE Mat33d(const Vec3d& column0, const Vec3d& column1, const Vec3d& column2)
        : column0(column0), column1(column1), column2(column2)
    {
    }

    CUDA_CALLABLE Mat33d(const Mat33& m)
    {
        column0 = m.column0;
        column1 = m.column1;
        column2 = m.column2;
    }

    CUDA_CALLABLE Mat33d(const QuatD& q);
    CUDA_CALLABLE Mat33d(_IDENTITY)
        : column0(Vec3d(1.0, 0.0, 0.0)), column1(Vec3d(0.0, 1.0, 0.0)), column2(Vec3d(0.0, 0.0, 1.0)){};

    CUDA_CALLABLE Mat33d(_ZERO) : column0(Vec3d(Zero)), column1(Vec3d(Zero)), column2(Vec3d(Zero)){};

    CUDA_CALLABLE Mat33d(const Vec3d& v)
        : column0(Vec3d(v.x, 0.0, 0.0)), column1(Vec3d(0.0, v.y, 0.0)), column2(Vec3d(0.0, 0.0, v.z)){};

    CUDA_CALLABLE Mat33d(
        double a00, double a01, double a02, double a10, double a11, double a12, double a20, double a21, double a22)
        : column0(Vec3d(a00, a10, a20)), column1(Vec3d(a01, a11, a21)), column2(Vec3d(a02, a12, a22))
    {
    }

    CUDA_CALLABLE bool isZero() const
    {
        return column0.isZero() && column1.isZero() && column2.isZero();
    }

    CUDA_CALLABLE Mat33d getTranspose() const
    {
        Vec3d v0(column0.x, column1.x, column2.x);
        Vec3d v1(column0.y, column1.y, column2.y);
        Vec3d v2(column0.z, column1.z, column2.z);
        return Mat33d(v0, v1, v2);
    }

    CUDA_CALLABLE Vec3d operator*(const Vec3d& v) const
    {
        return column0 * v.x + column1 * v.y + column2 * v.z;
    }

    CUDA_CALLABLE const Mat33d operator*(double scalar) const
    {
        return Mat33d(column0 * scalar, column1 * scalar, column2 * scalar);
    }

    CUDA_CALLABLE Mat33d operator*(const Mat33d& M) const
    {
        return Mat33d(*this * M.column0, *this * M.column1, *this * M.column2);
    }

    CUDA_CALLABLE Vec3d& operator[](unsigned int num)
    {
        return (&column0)[num];
    }
    CUDA_CALLABLE const Vec3d& operator[](unsigned int num) const
    {
        return (&column0)[num];
    }

    CUDA_CALLABLE double operator()(unsigned int row, unsigned int col) const
    {
        return (*this)[col][row];
    }

    CUDA_CALLABLE double& operator()(unsigned int row, unsigned int col)
    {
        return (*this)[col][row];
    }

    CUDA_CALLABLE const Mat33d operator+(const Mat33d& M) const
    {
        return Mat33d(column0 + M.column0, column1 + M.column1, column2 + M.column2);
    }

    CUDA_CALLABLE const Mat33d operator-(const Mat33d& M) const
    {
        return Mat33d(column0 - M.column0, column1 - M.column1, column2 - M.column2);
    }

    CUDA_CALLABLE Mat33d& operator+=(const Mat33d& M)
    {
        column0 += M.column0;
        column1 += M.column1;
        column2 += M.column2;
        return *this;
    }

    CUDA_CALLABLE Mat33d& operator-=(const Mat33d& M)
    {
        column0 -= M.column0;
        column1 -= M.column1;
        column2 -= M.column2;
        return *this;
    }

    CUDA_CALLABLE void operator*=(double scalar)
    {
        column0 *= scalar;
        column1 *= scalar;
        column2 *= scalar;
    }

    CUDA_CALLABLE void operator/=(double scalar)
    {
        column0 /= scalar;
        column1 /= scalar;
        column2 /= scalar;
    }

    CUDA_CALLABLE const Mat33d operator-() const
    {
        return Mat33d(-column0, -column1, -column2);
    }

    CUDA_CALLABLE bool operator==(const Mat33d& M) const
    {
        return column0 == M.column0 && column1 == M.column1 && column2 == M.column2;
    }

    CUDA_CALLABLE bool operator<(const Mat33d& M) const
    {
        if (column0 != M.column0)
            return column0 < M.column0;
        if (column1 != M.column1)
            return column1 < M.column1;
        return column2 < M.column2;
    }

    CUDA_CALLABLE double getDeterminant() const
    {
        return column0.dot(column1.cross(column2));
    }
    CUDA_CALLABLE const Mat33d getInverse() const
    {
        const double det = getDeterminant();
        Mat33d inverse;

        if (det != 0.0)
        {
            const double invDet = 1.0 / det;

            inverse.column0.x = invDet * (column1.y * column2.z - column2.y * column1.z);
            inverse.column0.y = invDet * -(column0.y * column2.z - column2.y * column0.z);
            inverse.column0.z = invDet * (column0.y * column1.z - column0.z * column1.y);

            inverse.column1.x = invDet * -(column1.x * column2.z - column1.z * column2.x);
            inverse.column1.y = invDet * (column0.x * column2.z - column0.z * column2.x);
            inverse.column1.z = invDet * -(column0.x * column1.z - column0.z * column1.x);

            inverse.column2.x = invDet * (column1.x * column2.y - column1.y * column2.x);
            inverse.column2.y = invDet * -(column0.x * column2.y - column0.y * column2.x);
            inverse.column2.z = invDet * (column0.x * column1.y - column1.x * column0.y);

            return inverse;
        }
        else
            return Mat33d(Identity);
    }

    CUDA_CALLABLE double dot(const Mat33d& M) const
    {
        return column0.dot(M.column0) + column1.dot(M.column1) + column2.dot(M.column2);
    }

    CUDA_CALLABLE void createFrame(const Vec3d& axis)
    {
        column0 = axis;
        column0.normalize();
        column1 = column0.perp();
        column2 = column0.cross(column1);
    }

    Vec3d column0, column1, column2;
};

CUDA_CALLABLE inline Mat33d operator*(double d, const Mat33d& M)
{
    return Mat33d(d * M.column0, d * M.column1, d * M.column2);
}

/**
 * @brief Defines a QuatDernion with the double components x, y, z and w, where x, y and z define the imaginary part and
 * w the real part.
 */
struct QuatD
{
    CUDA_CALLABLE QuatD() : x(0.0), y(0.0), z(0.0), w(0.0)
    {
    }
    CUDA_CALLABLE QuatD(double x, double y, double z, double w) : x(x), y(y), z(z), w(w)
    {
    }
    CUDA_CALLABLE QuatD(_IDENTITY)
    {
        x = y = z = 0.0;
        w = 1.0;
    }
    CUDA_CALLABLE QuatD(const Quat q) : x(q.x), y(q.y), z(q.z), w(q.w)
    {
    }
    CUDA_CALLABLE QuatD(const Vec3d& v, double w) : x(v.x), y(v.y), z(v.z), w(w)
    {
    }
    CUDA_CALLABLE QuatD(double angleRadians, const Vec3d& unitAxis)
    {
        const double a = angleRadians * 0.5f;
        const double s = sin(a);
        w = cos(a);
        x = unitAxis.x * s;
        y = unitAxis.y * s;
        z = unitAxis.z * s;
    }
    CUDA_CALLABLE QuatD(const Mat33d& m)
    {
        Vec3d c0 = m.column0.normalized();
        Vec3d c1 = m.column1.normalized();
        Vec3d c2 = m.column2.normalized();

        if (c2.z < 0.0)
        {
            if (c0.x > c1.y)
            {
                double t = 1.0 + c0.x - c1.y - c2.z;
                *this = QuatD(t, c0.y + c1.x, c2.x + c0.z, c1.z - c2.y) * (0.5 / sqrt(t));
            }
            else
            {
                double t = 1.0 - c0.x + c1.y - c2.z;
                *this = QuatD(c0.y + c1.x, t, c1.z + c2.y, c2.x - c0.z) * (0.5 / sqrt(t));
            }
        }
        else
        {
            if (c0.x < -c1.y)
            {
                double t = 1.0 - c0.x - c1.y + c2.z;
                *this = QuatD(c2.x + c0.z, c1.z + c2.y, t, c0.y - c1.x) * (0.5 / sqrt(t));
            }
            else
            {
                double t = 1.0 + c0.x + c1.y + c2.z;
                *this = QuatD(c1.z - c2.y, c2.x - c0.z, c0.y - c1.x, t) * (0.5 / sqrt(t));
            }
        }
    }
    CUDA_CALLABLE QuatD(const Vec3d& fromAxis, const Vec3d& toAxis)
    {
        Vec3d v0 = fromAxis, v1 = toAxis;
        v0.normalize();
        v1.normalize();
        double dot = v0.dot(v1);
        if (dot >= 1.0)
        {
            x = y = z = 0.0;
            w = 1.0;
        }
        else if (dot <= -1.0)
        {
            Vec3d p = v0.perp();
            x = p.x;
            y = p.y;
            z = p.z;
            w = 0.0;
        }
        else
        {
            double s = sqrt((1.0 + dot) * 2.0);
            double invs = 1.0 / s;
            Vec3d cross = v0.cross(v1);
            x = cross.x * invs;
            y = cross.y * invs;
            z = cross.z * invs;
            w = s * 0.5f;
            normalize();
        }
    }

    CUDA_CALLABLE Vec3d getBasisVector0() const
    {
        const double x2 = x * 2.0;
        const double w2 = w * 2.0;
        return Vec3d((w * w2) - 1.0 + x * x2, (z * w2) + y * x2, (-y * w2) + z * x2);
    }

    CUDA_CALLABLE Vec3d getBasisVector1() const
    {
        const double y2 = y * 2.0;
        const double w2 = w * 2.0;
        return Vec3d((-z * w2) + x * y2, (w * w2) - 1.0 + y * y2, (x * w2) + z * y2);
    }

    CUDA_CALLABLE Vec3d getBasisVector2() const
    {
        const double z2 = z * 2.0;
        const double w2 = w * 2.0;
        return Vec3d((y * w2) + x * z2, (-x * w2) + y * z2, (w * w2) - 1.0 + z * z2);
    }

    CUDA_CALLABLE QuatD operator*(double s) const
    {
        return QuatD(x * s, y * s, z * s, w * s);
    }

    CUDA_CALLABLE void operator+=(const QuatD& q)
    {
        x += q.x;
        y += q.y;
        z += q.z;
        w += q.w;
    }

    CUDA_CALLABLE QuatD operator+(const QuatD& q)
    {
        return QuatD(x + q.x, y + q.y, z + q.z, w + q.w);
    }

    CUDA_CALLABLE QuatD operator-(const QuatD& q)
    {
        return QuatD(x - q.x, y - q.y, z - q.z, w - q.w);
    }

    CUDA_CALLABLE double magnitude()
    {
        return sqrt(x * x + y * y + z * z + w * w);
    }
    CUDA_CALLABLE void normalize()
    {
        double s = 1.0 / magnitude();
        x *= s;
        y *= s, z *= s;
        w *= s;
    }

    CUDA_CALLABLE QuatD getConjugate() const
    {
        return QuatD(-x, -y, -z, w);
    }

    CUDA_CALLABLE QuatD getInverse() const
    {
        return QuatD(-x, -y, -z, w);
    }

    CUDA_CALLABLE const Vec3d rotate(const Vec3d& v) const
    {
        const double vx = 2.0 * v.x;
        const double vy = 2.0 * v.y;
        const double vz = 2.0 * v.z;
        const double w2 = w * w - 0.5f;
        const double dot2 = (x * vx + y * vy + z * vz);
        return Vec3d((vx * w2 + (y * vz - z * vy) * w + x * dot2), (vy * w2 + (z * vx - x * vz) * w + y * dot2),
                     (vz * w2 + (x * vy - y * vx) * w + z * dot2));
    }

    CUDA_CALLABLE const Vec3d rotateInv(const Vec3d& v) const
    {
        const double vx = 2.0 * v.x;
        const double vy = 2.0 * v.y;
        const double vz = 2.0 * v.z;
        const double w2 = w * w - 0.5f;
        const double dot2 = (x * vx + y * vy + z * vz);
        return Vec3d((vx * w2 - (y * vz - z * vy) * w + x * dot2), (vy * w2 - (z * vx - x * vz) * w + y * dot2),
                     (vz * w2 - (x * vy - y * vx) * w + z * dot2));
    }

    CUDA_CALLABLE QuatD operator*(const QuatD& q) const
    {
        return QuatD(w * q.x + q.w * x + y * q.z - q.y * z, w * q.y + q.w * y + z * q.x - q.z * x,
                     w * q.z + q.w * z + x * q.y - q.x * y, w * q.w - x * q.x - y * q.y - z * q.z);
    }

    CUDA_CALLABLE QuatD operator+(const QuatD& q) const
    {
        return QuatD(x + q.x, y + q.y, z + q.z, w + q.w);
    }

    CUDA_CALLABLE inline QuatD rotateLinear(const QuatD& q, const Vec3d& omega)
    {
        QuatD res = q;
        res += QuatD(omega.x, omega.y, omega.z, 0.0) * q * 0.5;
        res.normalize();
        return res;
    }

    CUDA_CALLABLE inline QuatD rotateNonlinear(const QuatD& q, const Vec3d& omega)
    {
        QuatD res = q;
        if (!omega.isZero())
        {
            Vec3d n = omega;
            double ang = n.normalize();
            res = QuatD(ang, n) * q;
        }
        return res;
    }

    CUDA_CALLABLE QuatD lerp(const QuatD& v, double s) const
    {
        double t = 1.0 - s;
        QuatD q(x * t + v.x * s, y * t + v.y * s, z * t + v.z * s, w * t + v.w * s);
        q.normalize();
        return q;
    }

    CUDA_CALLABLE QuatD slerp(const QuatD& v, double t) const
    {
        double cosHalfTheta = x * v.x + y * v.y + z * v.z + w * v.w;
        if (abs(cosHalfTheta) >= 1.0)
            return *this;

        bool reverse = false;
        if (cosHalfTheta < 0.0)
        {
            reverse = true;
            cosHalfTheta = -cosHalfTheta;
        }

        double halfTheta = Acos(cosHalfTheta);
        double sinHalfTheta = sqrt(1.0 - cosHalfTheta * cosHalfTheta);
        double a = 1.0 - t;
        double b = t;

        if (abs(sinHalfTheta) > 0.001)
        {
            a = Sin((1.0 - t) * halfTheta) / sinHalfTheta;
            b = Sin(t * halfTheta) / sinHalfTheta;
        }

        if (!reverse)
            return QuatD(a * x + b * v.x, a * y + b * v.y, a * z + b * v.z, a * w + b * v.w);
        else
            return QuatD(a * x - b * v.x, a * y - b * v.y, a * z - b * v.z, a * w - b * v.w);
    }

    double x, y, z, w;
};


CUDA_CALLABLE inline QuatD operator*(double d, const QuatD& q)
{
    return QuatD(d * q.x, d * q.y, d * q.z, d * q.w);
}


CUDA_CALLABLE inline Mat33d::Mat33d(const QuatD& q)
{
    double x = q.x, y = q.y, z = q.z, w = q.w;
    double x2 = x + x, y2 = y + y, z2 = z + z;
    double xx = x2 * x, yy = y2 * y, zz = z2 * z;
    double xy = x2 * y, xz = x2 * z, xw = x2 * w;
    double yz = y2 * z, yw = y2 * w, zw = z2 * w;
    column0 = Vec3d(1.0 - yy - zz, xy + zw, xz - yw);
    column1 = Vec3d(xy - zw, 1.0 - xx - zz, yz + xw);
    column2 = Vec3d(xz + yw, yz - xw, 1.0 - xx - yy);
}

/**
 * @brief Defines a rigid TransformDation via a translation and a rotation.
 */
struct TransformD
{
    /**
     * @var Vec3 p
     * @brief The translation of the transform.
     */

    /**
     * @brief Unit quaternion q
     * @brief The rotation of the transform.
     */

public:
    Vec3d p;
    QuatD q;

    CUDA_CALLABLE TransformD() : p(Zero), q(){};
    CUDA_CALLABLE TransformD(_IDENTITY) : p(Zero), q(Identity){};
    CUDA_CALLABLE TransformD(const Vec3d& p, const QuatD& q) : p(p), q(q){};
    CUDA_CALLABLE TransformD(const Vec3d& p) : p(p), q(Identity){};
    CUDA_CALLABLE TransformD(const QuatD& q) : p(Zero), q(q){};
    CUDA_CALLABLE TransformD getInverse() const
    {
        return TransformD(q.rotateInv(p * -1.0f), q.getConjugate());
    }

    CUDA_CALLABLE TransformD transform(const TransformD& t) const
    {
        return TransformD(q.rotate(t.p) + p, q * t.q);
    }

    CUDA_CALLABLE TransformD transformInv(const TransformD& t) const
    {
        QuatD qinv = q.getConjugate();
        return TransformD(qinv.rotate(t.p - p), qinv * t.q);
    }

    CUDA_CALLABLE Vec3d transform(const Vec3d& v) const
    {
        return q.rotate(v) + p;
    }

    CUDA_CALLABLE Vec3d transformInv(const Vec3d& v) const
    {
        return q.rotateInv(v - p);
    }

    CUDA_CALLABLE Vec3d rotate(const Vec3d& v) const
    {
        return q.rotate(v);
    }

    CUDA_CALLABLE Vec3d rotateInv(const Vec3d& v) const
    {
        return q.rotateInv(v);
    }

    CUDA_CALLABLE PlaneD transform(const PlaneD& in) const
    {
        PlaneD out;
        out.n = q.rotate(in.n);
        out.d = in.d + out.n.dot(p);
        return out;
    }

    CUDA_CALLABLE RayD transform(const RayD& r) const
    {
        return RayD(transform(r.orig), rotate(r.dir));
    }

    CUDA_CALLABLE RayD transformInv(const RayD& r) const
    {
        return RayD(transformInv(r.orig), rotateInv(r.dir));
    }

    CUDA_CALLABLE PlaneD transformInv(const PlaneD& in) const
    {
        PlaneD out;
        out.n = q.rotateInv(in.n);
        out.d = in.d - out.n.dot(q.rotateInv(p));
        return out;
    }

    CUDA_CALLABLE TransformD operator*(const TransformD& t) const
    {
        return transform(t);
    }
};

struct Mat44d;

/**
 * @brief Defines an affine TransformDation.
 * An affine TransformDation is a TransformDation that preserves straight lines and planes.
 * It can be represented as a 3x3 matrix A and a translation vector b
 * or as a 4x4 matrix with the last row being (0, 0, 0, 1).
 */

struct AffineTransformD
{
    /**
     * @var Mat33d A
     * @brief The linear TransformDation of the affine TransformD.
     */

    /**
     * @var Vec3d b
     * @brief The translation of the affine TransformD.
     */
public:
    Mat33d A;
    Vec3d b;

    CUDA_CALLABLE AffineTransformD() : A(Zero), b(Zero){};

    /**
     * @brief Defines an affine TransformDation which maps all points to the origin.
     */
    CUDA_CALLABLE AffineTransformD(_ZERO) : A(Zero), b(Zero){};

    /**
     * @brief Defines an affine TransformDation which maps all points to themselves.
     */
    CUDA_CALLABLE AffineTransformD(_IDENTITY) : A(Identity), b(Zero){};

    /**
     * @brief Defines an affine TransformDation with the given linear TransformDation and translation.
     */
    CUDA_CALLABLE AffineTransformD(const Mat33d& A, const Vec3d& b) : A(A), b(b){};

    /**
     * @brief Defines an affine TransformDation with the given rigid TransformD.
     */
    CUDA_CALLABLE AffineTransformD(const TransformD& t, const Vec3d& scale = Vec3d(1.0, 1.0, 1.0))
        : A(Mat33d(t.q) * Mat33d(scale)), b(t.p){};

    /**
     * @brief Defines an affine TransformDation with the given 4x4 matrix.
     */
    CUDA_CALLABLE AffineTransformD(const Mat44d m);

    /**
     * @brief Tests whether the affine TransformDation is the zero TransformDation.
     */
    CUDA_CALLABLE bool isZero() const
    {
        return A.isZero() && b.isZero();
    }

    /**
     * @brief TransformDs the given vector.
     */
    CUDA_CALLABLE Vec3d transform(const Vec3d& v) const
    {
        return A * v + b;
    }

    /**
     * @brief Transforms the given ray.
     */
    CUDA_CALLABLE RayD transform(const RayD& r) const
    {
        return RayD(transform(r.orig), A * r.dir);
    }

    CUDA_CALLABLE AffineTransformD transform(const AffineTransformD& t) const
    {
        return AffineTransformD(A * t.A, A * t.b + b);
    }

    /**
     * @brief Get the matrix M that can be used to TransformD normals such that n' = M n
     */
    CUDA_CALLABLE Mat33d getNormalMatrix() const
    {
        return A.getInverse().getTranspose();
    }

    /**
     * @brief Get the inverse of the affine transformation.
     */
    CUDA_CALLABLE AffineTransformD getInverse() const
    {
        Mat33d Ainv = A.getInverse();
        return AffineTransformD(Ainv, Ainv * -b);
    }

    /**
     * @brief Multiplies the affine TransformDation with a scalar.
     */
    CUDA_CALLABLE const AffineTransformD operator*(double scalar) const
    {
        return AffineTransformD(A * scalar, b * scalar);
    }

    /**
     * @brief Adds a given TransformD.
     */
    CUDA_CALLABLE void operator+=(const AffineTransformD& at)
    {
        A += at.A;
        b += at.b;
    }

    /**
     * @brief Subtracts a given TransformD.
     */
    CUDA_CALLABLE void operator-=(const AffineTransformD& at)
    {
        A -= at.A;
        b -= at.b;
    }

    CUDA_CALLABLE AffineTransformD operator*(const AffineTransformD& t) const
    {
        return transform(t);
    }
};

/**
 * @brief Defines a 4x4 matrix via its columns of type Vec4.
 */
struct Mat44d
{
    CUDA_CALLABLE Mat44d() : column0(Zero), column1(Zero), column2(Zero), column3(Zero)
    {
    }

    CUDA_CALLABLE Mat44d(const Mat44& m)
    {
        column0 = m.column0;
        column1 = m.column1;
        column2 = m.column2;
        column3 = m.column3;
    }

    CUDA_CALLABLE Mat44d(const Vec4d& column0, const Vec4d& column1, const Vec4d& column2, const Vec4d& column3)
        : column0(column0), column1(column1), column2(column2), column3(column3)
    {
    }

    CUDA_CALLABLE Mat44d(_IDENTITY)
        : column0(Vec4d(1.0, 0.0, 0.0, 0.0)),
          column1(Vec4d(0.0, 1.0, 0.0, 0.0)),
          column2(Vec4d(0.0, 0.0, 1.0, 0.0)),
          column3(Vec4d(0.0, 0.0, 0.0, 1.0)){};

    CUDA_CALLABLE Mat44d(_ZERO)
        : column0(Vec4d(Zero)), column1(Vec4d(Zero)), column2(Vec4d(Zero)), column3(Vec4d(Zero)){};

    CUDA_CALLABLE Mat44d(double m[])
        : column0(m[0], m[1], m[2], m[3]),
          column1(m[4], m[5], m[6], m[7]),
          column2(m[8], m[9], m[10], m[11]),
          column3(m[12], m[13], m[14], m[15])
    {
    }

    CUDA_CALLABLE Mat44d(double a00,
                         double a01,
                         double a02,
                         double a03,
                         double a10,
                         double a11,
                         double a12,
                         double a13,
                         double a20,
                         double a21,
                         double a22,
                         double a23,
                         double a30,
                         double a31,
                         double a32,
                         double a33)
        : column0(a00, a10, a20, a30),
          column1(a01, a11, a21, a31),
          column2(a02, a12, a22, a32),
          column3(a03, a13, a23, a33)
    {
    }

    CUDA_CALLABLE Mat44d(const TransformD& trans, const Vec3d& scale = Vec3d(1.0, 1.0, 1.0))
        : column0(trans.q.getBasisVector0() * scale.x),
          column1(trans.q.getBasisVector1() * scale.y),
          column2(trans.q.getBasisVector2() * scale.z),
          column3(trans.p, 1.0)
    {
    }

    CUDA_CALLABLE Mat44d(const Vec3d& col0, const Vec3d& col1, const Vec3d& col2, const Vec3d& col3)
        : column0(col0, 0), column1(col1, 0), column2(col2, 0), column3(col3, 1.0)
    {
    }

    CUDA_CALLABLE Mat44d(const AffineTransformD& t)
        : column0(t.A.column0), column1(t.A.column1), column2(t.A.column2), column3(Vec4d(t.b, 1.0))
    {
    }

    CUDA_CALLABLE void getColumnMajor(double* d) const
    {
        d[0] = column0.x;
        d[4] = column1.x;
        d[8] = column2.x;
        d[12] = column3.x;
        d[1] = column0.y;
        d[5] = column1.y;
        d[9] = column2.y;
        d[13] = column3.y;
        d[2] = column0.z;
        d[6] = column1.z;
        d[10] = column2.z;
        d[14] = column3.z;
        d[3] = column0.w;
        d[7] = column1.w;
        d[11] = column2.w;
        d[15] = column3.w;
    }

    CUDA_CALLABLE Mat44d getTranspose() const
    {
        Vec4d v0(column0.x, column1.x, column2.x, column3.x);
        Vec4d v1(column0.y, column1.y, column2.y, column3.y);
        Vec4d v2(column0.z, column1.z, column2.z, column3.z);
        Vec4d v3(column0.w, column1.w, column2.w, column3.w);
        return Mat44d(v0, v1, v2, v3);
    }

    CUDA_CALLABLE const Mat44d getInverse() const
    {
        double inv[16], m[16];
        getColumnMajor(m);

        inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] +
                 m[13] * m[6] * m[11] - m[13] * m[7] * m[10];

        inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] -
                 m[12] * m[6] * m[11] + m[12] * m[7] * m[10];

        inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] +
                 m[12] * m[5] * m[11] - m[12] * m[7] * m[9];

        inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] -
                  m[12] * m[5] * m[10] + m[12] * m[6] * m[9];

        inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] -
                 m[13] * m[2] * m[11] + m[13] * m[3] * m[10];

        inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] +
                 m[12] * m[2] * m[11] - m[12] * m[3] * m[10];

        inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] -
                 m[12] * m[1] * m[11] + m[12] * m[3] * m[9];

        inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] +
                  m[12] * m[1] * m[10] - m[12] * m[2] * m[9];

        inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] +
                 m[13] * m[2] * m[7] - m[13] * m[3] * m[6];

        inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] -
                 m[12] * m[2] * m[7] + m[12] * m[3] * m[6];

        inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] +
                  m[12] * m[1] * m[7] - m[12] * m[3] * m[5];

        inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] -
                  m[12] * m[1] * m[6] + m[12] * m[2] * m[5];

        inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] -
                 m[9] * m[2] * m[7] + m[9] * m[3] * m[6];

        inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] +
                 m[8] * m[2] * m[7] - m[8] * m[3] * m[6];

        inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] -
                  m[8] * m[1] * m[7] + m[8] * m[3] * m[5];

        inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] +
                  m[8] * m[1] * m[6] - m[8] * m[2] * m[5];

        double det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

        if (det == 0)
            return Mat44d(Zero);

        det = 1.0 / det;

        for (int i = 0; i < 16; i++)
            inv[i] *= det;

        return Mat44d(inv);
    }

    CUDA_CALLABLE Vec4d transform(const Vec4d& v) const
    {
        return column0 * v.x + column1 * v.y + column2 * v.z + column3 * v.w;
    }

    CUDA_CALLABLE Mat33d get33() const
    {
        return Mat33d(column0.getXYZ(), column1.getXYZ(), column2.getXYZ());
    }

    CUDA_CALLABLE Vec3d getP() const
    {
        return column3.getXYZ();
    }

    CUDA_CALLABLE TransformD getRigidTransform() const
    {
        return TransformD(column3.getXYZ(), QuatD(get33()));
    }

    CUDA_CALLABLE Vec4d operator*(const Vec4d& v) const
    {
        return transform(v);
    }

    CUDA_CALLABLE Mat44d operator*(double scalar) const
    {
        return Mat44d(column0 * scalar, column1 * scalar, column2 * scalar, column3 * scalar);
    }

    CUDA_CALLABLE Mat44d operator*(const Mat44d& other) const
    {
        return Mat44d(
            transform(other.column0), transform(other.column1), transform(other.column2), transform(other.column3));
    }

    CUDA_CALLABLE Mat44d operator+(const Mat44d& M) const
    {
        return Mat44d(column0 + M.column0, column1 + M.column1, column2 + M.column2, column3 + M.column3);
    }

    CUDA_CALLABLE Mat44d operator-(const Mat44d& M) const
    {
        return Mat44d(column0 - M.column0, column1 - M.column1, column2 - M.column2, column3 - M.column3);
    }

    CUDA_CALLABLE Mat44d& operator+=(const Mat44d& M)
    {
        column0 += M.column0;
        column1 += M.column1;
        column2 += M.column2;
        column3 += M.column3;
        return *this;
    }

    CUDA_CALLABLE Mat44d& operator-=(const Mat44d& M)
    {
        column0 -= M.column0;
        column1 -= M.column1;
        column2 -= M.column2;
        column3 -= M.column3;
        return *this;
    }

    CUDA_CALLABLE bool operator==(const Mat44d& M) const
    {
        return column0 == M.column0 && column1 == M.column1 && column2 == M.column2 && column3 == M.column3;
    }

    CUDA_CALLABLE bool operator<(const Mat44d& M) const
    {
        if (column0 != M.column0)
            return column0 < M.column0;
        if (column1 != M.column1)
            return column1 < M.column1;
        if (column2 != M.column2)
            return column2 < M.column2;
        return column3 < M.column3;
    }

    CUDA_CALLABLE Vec3d transform(const Vec3d& v) const
    {
        return transform(Vec4d(v, 1.0)).getXYZ();
    }

    CUDA_CALLABLE Vec3d rotate(const Vec3d& v) const
    {
        return transform(Vec4d(v, 0.0)).getXYZ();
    }

    CUDA_CALLABLE Vec4d& getColumn(int nr)
    {
        return *(&column0 + nr);
    }
    CUDA_CALLABLE const Vec4d& getColumn(int nr) const
    {
        return *(&column0 + nr);
    }

    Vec4d column0, column1, column2, column3;
};

CUDA_CALLABLE inline AffineTransformD::AffineTransformD(const Mat44d m)
    : A(m.column0.getXYZ(), m.column1.getXYZ(), m.column2.getXYZ()), b(m.column3.getXYZ()){};

CUDA_CALLABLE inline Mat44d operator*(double d, const Mat44d& M)
{
    return Mat44d(d * M.column0, d * M.column1, d * M.column2, d * M.column3);
}

/**
 * @brief Defines a two dimensional bounding box.
 */
struct Bounds2d
{
    /**
     * @var Vec2d minimum
     * @brief the minimum point of the bounding box.
     */
    /**
     * @var Vec2d maximum
     * @brief the maximum point of the bounding box.
     */

    CUDA_CALLABLE Bounds2d() : minimum(Zero), maximum(Zero)
    {
    }
    CUDA_CALLABLE Bounds2d(const Vec2d& minimum, const Vec2d& maximum) : minimum(minimum), maximum(maximum)
    {
    }
    CUDA_CALLABLE Bounds2d(const Vec2d& p) : minimum(p), maximum(p)
    {
    }
    CUDA_CALLABLE Bounds2d(_EMPTY)
    {
        setEmpty();
    }
    CUDA_CALLABLE Bounds2d(_ZERO) : minimum(Vec2d(Zero)), maximum(Vec2d(Zero))
    {
    }

    CUDA_CALLABLE void setEmpty()
    {
        minimum = Vec2d(MaxDouble, MaxDouble);
        maximum = Vec2d(-MaxDouble, -MaxDouble);
    }
    CUDA_CALLABLE bool isEmpty()
    {
        return minimum.x > maximum.x;
    }

    CUDA_CALLABLE void include(const Vec2d& p)
    {
        minimum.setMin(p);
        maximum.setMax(p);
    }

    CUDA_CALLABLE void include(const Bounds2d& b)
    {
        minimum.setMin(b.minimum);
        maximum.setMax(b.maximum);
    }

    CUDA_CALLABLE void expand(double d)
    {
        minimum -= Vec2d(d, d);
        maximum += Vec2d(d, d);
    }

    CUDA_CALLABLE bool intersect(const Bounds2d& b) const
    {
        return !(b.minimum.x > maximum.x || minimum.x > b.maximum.x || b.minimum.y > maximum.y ||
                 minimum.y > b.maximum.y);
    }

    CUDA_CALLABLE bool contain(const Vec2d& v) const
    {
        return !(v.x < minimum.x || v.x > maximum.x || v.y < minimum.y || v.y > maximum.y);
    }

    CUDA_CALLABLE Vec2d getDimensions() const
    {
        return maximum - minimum;
    }

    CUDA_CALLABLE Vec2d getHalfExtents() const
    {
        return getDimensions() * 0.5;
    }

    CUDA_CALLABLE Vec2d getCenter() const
    {
        return (maximum + minimum) * 0.5;
    }

    Vec2d minimum, maximum;
};


/**
 * @brief Defines a three dimensional bounding box.
 */
struct Bounds3d
{
    /**
     * @var Vec3 minimum
     * @brief the minimum point of the bounding box.
     */
    /**
     * @var Vec3 maximum
     * @brief the maximum point of the bounding box.
     */

    CUDA_CALLABLE Bounds3d() : minimum(Zero), maximum(Zero)
    {
    }
    CUDA_CALLABLE Bounds3d(const Vec3d& minimum, const Vec3d& maximum) : minimum(minimum), maximum(maximum)
    {
    }
    CUDA_CALLABLE Bounds3d(const Vec3d& p) : minimum(p), maximum(p)
    {
    }
    CUDA_CALLABLE Bounds3d(_EMPTY)
    {
        setEmpty();
    }
    CUDA_CALLABLE Bounds3d(_ZERO) : minimum(Vec3d(Zero)), maximum(Vec3d(Zero))
    {
    }
    CUDA_CALLABLE Bounds3d(const Bounds3& b) : minimum(b.minimum), maximum(b.maximum)
    {
    }
    CUDA_CALLABLE Bounds3d(const TransformD& pose, const Vec3d& halfExtents)
    {
        Vec3d c0 = pose.q.getBasisVector0() * halfExtents.x;
        Vec3d c1 = pose.q.getBasisVector1() * halfExtents.y;
        Vec3d c2 = pose.q.getBasisVector2() * halfExtents.z;
        Vec3d w;
        w.x = fabs(c0.x) + fabs(c1.x) + fabs(c2.x);
        w.y = fabs(c0.y) + fabs(c1.y) + fabs(c2.y);
        w.z = fabs(c0.z) + fabs(c1.z) + fabs(c2.z);
        minimum = pose.p - w;
        maximum = pose.p + w;
    }
    CUDA_CALLABLE Bounds3d(const AffineTransformD& pose, const Vec3d& halfExtents)
    {
        Vec3d c0 = pose.A.column0 * halfExtents.x;
        Vec3d c1 = pose.A.column1 * halfExtents.y;
        Vec3d c2 = pose.A.column2 * halfExtents.z;
        Vec3d w;
        w.x = fabs(c0.x) + fabs(c1.x) + fabs(c2.x);
        w.y = fabs(c0.y) + fabs(c1.y) + fabs(c2.y);
        w.z = fabs(c0.z) + fabs(c1.z) + fabs(c2.z);
        minimum = pose.b - w;
        maximum = pose.b + w;
    }
    CUDA_CALLABLE void setEmpty()
    {
        minimum = Vec3d(MaxDouble, MaxDouble, MaxDouble);
        maximum = Vec3d(-MaxDouble, -MaxDouble, -MaxDouble);
    }
    CUDA_CALLABLE bool isEmpty()
    {
        return minimum.x > maximum.x;
    }

    CUDA_CALLABLE void include(const Vec3d& p)
    {
        minimum.setMin(p);
        maximum.setMax(p);
    }

    CUDA_CALLABLE void include(const Bounds3d& b)
    {
        minimum.setMin(b.minimum);
        maximum.setMax(b.maximum);
    }

    CUDA_CALLABLE void expand(double d)
    {
        minimum -= Vec3d(d, d, d);
        maximum += Vec3d(d, d, d);
    }

    CUDA_CALLABLE bool intersect(const Bounds3d& b) const
    {
        return !(b.minimum.x > maximum.x || minimum.x > b.maximum.x || b.minimum.y > maximum.y ||
                 minimum.y > b.maximum.y || b.minimum.z > maximum.z || minimum.z > b.maximum.z);
    }

    CUDA_CALLABLE bool contain(const Vec3d& v) const
    {
        return !(v.x < minimum.x || v.x > maximum.x || v.y < minimum.y || v.y > maximum.y || v.z < minimum.z ||
                 v.z > maximum.z);
    }

    CUDA_CALLABLE Vec3d getDimensions() const
    {
        return maximum - minimum;
    }

    CUDA_CALLABLE Vec3d getHalfExtents() const
    {
        return getDimensions() * 0.5;
    }

    CUDA_CALLABLE Vec3d getCenter() const
    {
        return (maximum + minimum) * 0.5;
    }

    CUDA_CALLABLE Bounds3d transform(const TransformD& trans) const
    {
        return Bounds3d(TransformD(trans.transform(getCenter()), trans.q), getHalfExtents());
    }

    CUDA_CALLABLE Bounds3d transformInv(const TransformD& trans) const
    {
        return transform(trans.getInverse());
    }

    CUDA_CALLABLE Bounds3d transform(const AffineTransformD& trans) const
    {
        return Bounds3d(AffineTransformD(trans.A, trans.transform(getCenter())), getHalfExtents());
    }

    Vec3d minimum, maximum;
};


/**
 * @brief Defines a two dimensional oriented bounding box.
 */
struct OBB2d
{
    /**
     * @var Transform trans
     * @brief The rigid transformation of the zero centered bounding box.
     */
    /**
     * @var Vec3 halfExtents
     * @brief The half edge lengths of the bounding box.
     */

    CUDA_CALLABLE OBB2d()
    {
    }
    CUDA_CALLABLE OBB2d(const Vec2d& center, const Vec2d& axis, const Vec2d& halfExtents)
        : center(center), axis(axis), halfExtents(halfExtents)
    {
    }
    CUDA_CALLABLE OBB2d(_EMPTY)
    {
        setEmpty();
    }

    CUDA_CALLABLE void setEmpty()
    {
        center = Vec2d(Zero);
        axis = Vec2d(Zero);
        halfExtents = Vec2d(Zero);
    }

    CUDA_CALLABLE bool isEmpty()
    {
        return halfExtents.isZero();
    }

    bool contains(const Vec2d& p) const
    {
        double x = (p - center).dot(axis);
        double y = (p - center).cross(axis.perp());
        return fabs(x) <= halfExtents.x && fabs(y) <= halfExtents.y;
    }

    Vec2d center;
    Vec2d axis;
    Vec2d halfExtents;
};


/**
 * @brief Defines a three dimensional oriented bounding box.
 */
struct OBBd
{
    /**
     * @var TransformD trans
     * @brief The rigid TransformDation of the zero centered bounding box.
     */
    /**
     * @var Vec3d halfExtents
     * @brief The half edge lengths of the bounding box.
     */

    CUDA_CALLABLE OBBd()
    {
    }
    CUDA_CALLABLE OBBd(const TransformD& trans, const Vec3d& halfExtents) : trans(trans), halfExtents(halfExtents)
    {
    }
    CUDA_CALLABLE OBBd(_EMPTY)
    {
        setEmpty();
    }

    CUDA_CALLABLE void setEmpty()
    {
        trans = TransformD(Identity);
        halfExtents = Vec3d(Zero);
    }

    CUDA_CALLABLE bool isEmpty()
    {
        return halfExtents.isZero();
    }

    bool contains(const Vec3d& p) const
    {
        Vec3d localP = trans.transformInv(p);
        return fabs(localP.x) <= halfExtents.x && fabs(localP.y) <= halfExtents.y && fabs(localP.z) <= halfExtents.z;
    }

    TransformD trans;
    Vec3d halfExtents;
};


// --------------------------------------------------------------------------
/**
 * @brief Computes the outer product v0 v1^T of two vectors which yields a matrix.
 */
inline Mat33d outerProduct(const Vec3d& v0, const Vec3d& v1)
{
    Mat33d M;
    M.column0 = v0 * v1.x;
    M.column1 = v0 * v1.y;
    M.column2 = v0 * v1.z;
    return M;
}

/**
 * @brief Defines a quadric error measure used for mesh simplification.
 */
struct Quadricd
{
public:
    CUDA_CALLABLE void zero()
    {
        a00 = 0.0;
        a01 = 0.0;
        a02 = 0.0;
        a03 = 0.0;
        a11 = 0.0;
        a12 = 0.0;
        a13 = 0.0;
        a22 = 0.0;
        a23 = 0.0;
        a33 = 0.0;
    }

    // generate quadric from plane
    CUDA_CALLABLE void setFromPlane(const Vec3d& v0, const Vec3d& v1, const Vec3d& v2)
    {
        Vec3d n = (v1 - v0).cross(v2 - v0);
        n.normalize();
        double d = -n.dot(v0);
        a00 = n.x * n.x;
        a01 = n.x * n.y;
        a02 = n.x * n.z;
        a03 = n.x * d;
        a11 = n.y * n.y;
        a12 = n.y * n.z;
        a13 = n.y * d;
        a22 = n.z * n.z;
        a23 = n.z * d;
        a33 = d * d;
    }

    CUDA_CALLABLE Quadricd operator+(const Quadricd& q) const
    {
        Quadricd sum;
        sum.a00 = a00 + q.a00;
        sum.a01 = a01 + q.a01;
        sum.a02 = a02 + q.a02;
        sum.a03 = a03 + q.a03;
        sum.a11 = a11 + q.a11;
        sum.a12 = a12 + q.a12;
        sum.a13 = a13 + q.a13;
        sum.a22 = a22 + q.a22;
        sum.a23 = a23 + q.a23;
        sum.a33 = a33 + q.a33;
        return sum;
    }

    CUDA_CALLABLE Quadricd operator-(const Quadricd& q) const
    {
        Quadricd sum;
        sum.a00 = a00 - q.a00;
        sum.a01 = a01 - q.a01;
        sum.a02 = a02 - q.a02;
        sum.a03 = a03 - q.a03;
        sum.a11 = a11 - q.a11;
        sum.a12 = a12 - q.a12;
        sum.a13 = a13 - q.a13;
        sum.a22 = a22 - q.a22;
        sum.a23 = a23 - q.a23;
        sum.a33 = a33 - q.a33;
        return sum;
    }

    CUDA_CALLABLE void operator+=(const Quadricd& q)
    {
        a00 += q.a00;
        a01 += q.a01;
        a02 += q.a02;
        a03 += q.a03;
        a11 += q.a11;
        a12 += q.a12;
        a13 += q.a13;
        a22 += q.a22;
        a23 += q.a23;
        a33 += q.a33;
    }

    CUDA_CALLABLE double outerProduct(const Vec3d& v)
    {
        double vx = v.x;
        double vy = v.y;
        double vz = v.z;
        double res = a00 * vx * vx + 2.0 * a01 * vx * vy + 2.0 * a02 * vx * vz + 2.0 * a03 * vx + a11 * vy * vy +
                     2.0 * a12 * vy * vz + 2.0 * a13 * vy + a22 * vz * vz + 2.0 * a23 * vz + a33;
        return (double)res;
    }

    CUDA_CALLABLE double computeMinimum(const RayD& ray)
    {
        double ax = ray.orig.x;
        double ay = ray.orig.y;
        double az = ray.orig.z;

        double bx = ray.dir.x;
        double by = ray.dir.y;
        double bz = ray.dir.z;

        double a = a00 * bx * bx + a11 * by * by + a22 * bz * bz + 2.0 * a01 * bx * by + 2.0 * a02 * bx * bz +
                   2.0 * a12 * by * bz;

        double b = a00 * 2.0 * ax * bx + a11 * 2.0 * ay * by + a22 * 2.0 * az * bz + 2.0 * a01 * (ax * by + ay * bx) +
                   2.0 * a02 * (ax * bz + az * bx) + 2.0 * a12 * (ay * bz + az * by) + 2.0 * a03 * bx + 2.0 * a13 * by +
                   2.0 * a23 * bz;

        return a != 0.0 ? (double)(-b / (2.0 * a)) : 0.0;
    }

    double a00, a01, a02, a03;
    double a11, a12, a13;
    double a22, a23;
    double a33;
};

/**
 * @brief Defines a vector of general length
 */
struct Vecd
{
    Vecd(size_t size = 0)
    {
        data.resize(size, 0.0);
    }

    Vecd(const Vecd& v)
    {
        data.resize(v.data.size());
        for (size_t i = 0; i < data.size(); ++i)
        {
            data[i] = v.data[i];
        }
    }

    size_t size() const
    {
        return data.size();
    }

    void resize(size_t size)
    {
        data.resize(size, 0.0);
    }

    void setZero()
    {
        for (size_t i = 0; i < data.size(); ++i)
        {
            data[i] = 0.0;
        }
    }

    double& operator[](unsigned int index)
    {
        return data[index];
    }

    const double& operator[](unsigned int index) const
    {
        return data[index];
    }

    void get(Vec2d& v) const
    {
        v.x = data.size() > 0 ? data[0] : 0.0;
        v.y = data.size() > 1 ? data[1] : 0.0;
    }

    void get(Vec3d& v) const
    {
        v.x = data.size() > 0 ? data[0] : 0.0;
        v.y = data.size() > 1 ? data[1] : 0.0;
        v.z = data.size() > 2 ? data[2] : 0.0;
    }

    void get(Vec4d& v) const
    {
        v.x = data.size() > 0 ? data[0] : 0.0;
        v.y = data.size() > 1 ? data[1] : 0.0;
        v.z = data.size() > 2 ? data[2] : 0.0;
        v.w = data.size() > 3 ? data[3] : 0.0;
    }

    Vec2d getVec2d() const
    {
        Vec2d v;
        get(v);
        return v;
    }

    Vec3d getVec3d() const
    {
        Vec3d v;
        get(v);
        return v;
    }

    Vec4d getVec4d() const
    {
        Vec4d v;
        get(v);
        return v;
    }

    void set(const Vec2d& v)
    {
        data.resize(2);
        data[0] = v.x;
        data[1] = v.y;
    }

    void set(const Vec3d& v)
    {
        data.resize(3);
        data[0] = v.x;
        data[1] = v.y;
        data[2] = v.z;
    }

    void set(const Vec4d& v)
    {
        data.resize(4);
        data[0] = v.x;
        data[1] = v.y;
        data[2] = v.z;
        data[3] = v.w;
    }

    bool operator==(const Vecd& v) const
    {
        if (data.size() != v.data.size())
        {
            return false;
        }
        for (size_t i = 0; i < data.size(); ++i)
        {
            if (data[i] != v.data[i])
                return false;
        }
        return true;
    }

    bool operator!=(const Vecd& v) const
    {
        return !(*this == v);
    }

    bool isZero() const
    {
        for (size_t i = 0; i < data.size(); ++i)
        {
            if (data[i] != 0.0)
                return false;
        }
        return true;
    }

    Vecd operator-() const
    {
        Vecd v(data.size());
        for (size_t i = 0; i < data.size(); ++i)
        {
            v.data[i] = -data[i];
        }
        return v;
    }

    Vecd operator+(const Vecd& v) const
    {
        size_t newSize = Max(data.size(), v.data.size());
        Vecd r(newSize);
        for (size_t i = 0; i < newSize; ++i)
        {
            double a = i < data.size() ? data[i] : 0.0;
            double b = i < v.data.size() ? v.data[i] : 0.0;
            r.data[i] = a + b;
        }
        return r;
    }

    Vecd operator-(const Vecd& v) const
    {
        size_t newSize = Max(data.size(), v.data.size());
        Vecd r(newSize);
        for (size_t i = 0; i < newSize; ++i)
        {
            double a = i < data.size() ? data[i] : 0.0;
            double b = i < v.data.size() ? v.data[i] : 0.0;
            r.data[i] = a - b;
        }
        return r;
    }

    void operator+=(const Vecd& v)
    {
        for (size_t i = 0; i < Min(data.size(), v.data.size()); ++i)
        {
            data[i] += v.data[i];
        }
    }

    void operator-=(const Vecd& v)
    {
        for (size_t i = 0; i < Min(data.size(), v.data.size()); ++i)
        {
            data[i] -= v.data[i];
        }
    }

    void operator*=(double d)
    {
        for (size_t i = 0; i < data.size(); ++i)
        {
            data[i] *= d;
        }
    }

    void operator/=(double d)
    {
        for (size_t i = 0; i < data.size(); ++i)
        {
            data[i] /= d;
        }
    }

    Vecd operator*(double d) const
    {
        Vecd r(data.size());
        for (size_t i = 0; i < data.size(); ++i)
        {
            r.data[i] = data[i] * d;
        }
        return r;
    }

    Vecd operator/(double d) const
    {
        Vecd r(data.size());
        for (size_t i = 0; i < data.size(); ++i)
        {
            r.data[i] = data[i] / d;
        }
        return r;
    }

    double length() const
    {
        return sqrt(lengthSquared());
    }

    double lengthSquared() const
    {
        double l = 0.0;
        for (size_t i = 0; i < data.size(); ++i)
        {
            l += data[i] * data[i];
        }
        return l;
    }

    double normalize()
    {
        double l = length();
        if (l > 0.0)
        {
            *this /= l;
        }
        return l;
    }

    std::vector<double> data;
};

/**
 * @brief Adds a double value to an double variable. The operation is atomic if the code is executed on the GPU.
 * @param var The variable to which the value is added.
 * @param val The value to add.
 * @return The value of the variable before the addition.
 */
CUDA_CALLABLE_DEVICE inline double AtomicAdd(double* var, double val)
{
#ifdef __CUDACC__
    return atomicAdd(var, val);
#else
    *var += val;
    return *var - val;
#endif
}

/**
 * @brief Adds a Vec3d value to an Vec3d variable. The operation is atomic if the code is executed on the GPU.
 * @param var The variable to which the value is added.
 * @param val The value to add.
 * @return The value of the variable before the addition.
 */
CUDA_CALLABLE_DEVICE inline Vec3d AtomicAdd(Vec3d* v, Vec3d val)
{
    Vec3d old;
#ifdef __CUDACC__
    old.x = atomicAdd(&v->x, val.x);
    old.y = atomicAdd(&v->y, val.y);
    old.z = atomicAdd(&v->z, val.z);
    return old;
#else
    *v += val;
    return *v - val;
#endif
}

/**
 * @brief Adds a QuatDdvalue to an QuatDdvariable. The operation is atomic if the code is executed on the GPU.
 * @param var The variable to which the value is added.
 * @param val The value to add.
 * @return The value of the variable before the addition.
 */
CUDA_CALLABLE_DEVICE inline QuatD AtomicAdd(QuatD* q, QuatD val)
{
    QuatD old;
#ifdef __CUDACC__
    old.x = atomicAdd(&q->x, val.x);
    old.y = atomicAdd(&q->y, val.y);
    old.z = atomicAdd(&q->z, val.z);
    old.w = atomicAdd(&q->w, val.w);
    return old;
#else
    *q += val;
    return *q - val;
#endif
}

/**
 * @brief Adds a Quadric to a Quadric variable. The operation is atomic if the code is executed on the GPU.
 * @param var The variable to which the value is added.
 * @param val The value to add.
 * @return The value of the variable before the addition.
 */
CUDA_CALLABLE_DEVICE inline Quadricd AtomicAdd(Quadricd* q, Quadricd val)
{
    Quadricd old;
#ifdef __CUDACC__
    old.a00 = atomicAdd(&q->a00, val.a00);
    old.a01 = atomicAdd(&q->a01, val.a01);
    old.a02 = atomicAdd(&q->a02, val.a02);
    old.a03 = atomicAdd(&q->a03, val.a03);
    old.a11 = atomicAdd(&q->a11, val.a11);
    old.a12 = atomicAdd(&q->a12, val.a12);
    old.a13 = atomicAdd(&q->a13, val.a13);
    old.a22 = atomicAdd(&q->a22, val.a22);
    old.a23 = atomicAdd(&q->a23, val.a23);
    old.a33 = atomicAdd(&q->a33, val.a33);
    return old;
#else
    *q += val;
    old = *q - val;
#endif
    return old;
}

CUDA_CALLABLE inline Vec3 toFloat(const Vec3d& v)
{
    return Vec3((float)v.x, (float)v.y, (float)v.z);
}

CUDA_CALLABLE inline Vec4 toFloat(const Vec4d& v)
{
    return Vec4((float)v.x, (float)v.y, (float)v.z, (float)v.w);
}

CUDA_CALLABLE inline Quat toFloat(const QuatD& q)
{
    return Quat((float)q.x, (float)q.y, (float)q.z, (float)q.w);
}

CUDA_CALLABLE inline Transform toFloat(const TransformD& trans)
{
    return Transform(toFloat(trans.p), toFloat(trans.q));
}

CUDA_CALLABLE inline Mat33 toFloat(const Mat33d& mat)
{
    return Mat33(toFloat(mat.column0), toFloat(mat.column1), toFloat(mat.column2));
}

CUDA_CALLABLE inline Mat44 toFloat(const Mat44d& mat)
{
    return Mat44(toFloat(mat.column0), toFloat(mat.column1), toFloat(mat.column2), toFloat(mat.column3));
}

}
