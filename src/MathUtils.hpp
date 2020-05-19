/**
 *                                 [PHOTIC]
 *                                  v3.2.0
 *
 * This file is part of Photic, a collection of utilities for writing high-power
 * rocket flight computer software. Developed in Austin, TX by the Longhorn
 * Rocketry Association at the University of Texas at Austin.
 *
 *                            ---- THIS FILE ----
 *
 * Math utilities used across Photic.
 */

#ifndef PHOTIC_MATH_UTILS_HPP
#define PHOTIC_MATH_UTILS_HPP

#include "Matrix.hpp"

namespace Photic
{

namespace MathUtils
{
    /**
     * Makes a 2x2 matrix.
     *
     * @param   eij (i, j)th element in matrix.
     *
     * @ret     Constructed matrix.
     */
    inline Matrix<2, 2> makeMatrix2 (const Real_t e00, const Real_t e01,
                                     const Real_t e10, const Real_t e11)
    {
        Matrix<2, 2> mat;

        mat.mData[0] = e00; mat.mData[1] = e01;
        mat.mData[2] = e10; mat.mData[3] = e11;

        return mat;
    }

    /**
     * Makes a 3x3 matrix.
     *
     * @param   eij (i, j)th element in matrix.
     *
     * @ret     Constructed matrix.
     */
    inline Matrix<3, 3> makeMatrix3 (const Real_t e00, const Real_t e01, const Real_t e02,
                                     const Real_t e10, const Real_t e11, const Real_t e12,
                                     const Real_t e20, const Real_t e21, const Real_t e22)
    {
        Matrix<3, 3> mat;

        mat.mData[0] = e00; mat.mData[1] = e01; mat.mData[2] = e02;
        mat.mData[3] = e10; mat.mData[4] = e11; mat.mData[5] = e12;
        mat.mData[6] = e20; mat.mData[7] = e21; mat.mData[8] = e22;

        return mat;
    }

    /**
     * Makes a 2-vector.
     *
     * @param   x X component.
     * @param   y Y component.
     *
     * @ret     2-vector (x, y).
     */
    inline Matrix<2, 1> makeVector2 (const Real_t x, const Real_t y)
    {
        Matrix<2, 1> mat;

        mat.mData[0] = x;
        mat.mData[1] = y;

        return mat;
    }

    /**
     * Makes a 3-vector.
     *
     * @param   x X component.
     * @param   y Y component.
     * @param   z Z component.
     *
     * @ret     3-vector (x, y, z).
     */
    inline Matrix<3, 1> makeVector3 (const Real_t x, const Real_t y,
                                     const Real_t z)
    {
        Matrix<3, 1> mat;

        mat.mData[0] = x;
        mat.mData[1] = y;
        mat.mData[2] = z;

        return mat;
    }

    /**
     * Makes a 4-vector.
     *
     * @param   w W component.
     * @param   x X component.
     * @param   y Y component.
     * @param   z Z component.
     *
     * @ret     4-vector (w, x, y, z).
     */
    inline Matrix<4, 1> makeVector4 (const Real_t w, const Real_t x,
                                     const Real_t y, const Real_t z)
    {
        Matrix<4, 1> mat;

        mat.mData[0] = w;
        mat.mData[1] = x;
        mat.mData[2] = y;
        mat.mData[3] = z;

        return mat;
    }

    /**
     * Inverts a 2x2 matrix.
     *
     * @param   kMat Matrix to invert.
     *
     * @ret     Inverted matrix.
     */
    inline Matrix<2, 2> invertMatrix2 (const Matrix<2, 2>& kMat)
    {
        Real_t determinant = kMat (0, 0) * kMat (1, 1) -
                             kMat (0, 1) * kMat (1, 0);
        return makeMatrix2 ( kMat (1, 1) / determinant, -kMat (0, 1) / determinant,
                            -kMat (1, 0) / determinant,  kMat (0, 0) / determinant);
    }

    /**
     * Computes the cross product of two 3-vectors.
     *
     * @param   kLhs LHS vector.
     * @param   kRhs RHS vector.
     *
     * @ret     LHS cross RHS.
     */
    inline Vector3_t cross (const Vector3_t& kLhs, const Vector3_t& kRhs)
    {
        return makeVector3 (kLhs[1] * kRhs[2] - kLhs[2] * kRhs[1],
                            kLhs[2] * kRhs[0] - kLhs[0] * kRhs[2],
                            kLhs[0] * kRhs[1] - kLhs[1] * kRhs[0]);
    }

    /**
     * Rotates a 3-vector by a quaternion.
     *
     * WARNING: Quaternion must be normalized for a correct answer.
     *
     * @param   kQuat Quaternion ordered <w, x, y, z>.
     * @param   kVec  Vector to rotate.
     *
     * @ret     Rotated vector.
     */
    inline Vector3_t rotateVector (const Vector4_t& kQuat,
                                   const Vector3_t& kVec)
    {
        Vector3_t q = makeVector3 (kQuat[1], kQuat[2], kQuat[3]);
        Vector3_t t = cross (q, kVec) * 2.0;
        return kVec + t * kQuat[0] + cross (q, t);
    }

} // namespace MathUtils

} // namespace Photic

#endif