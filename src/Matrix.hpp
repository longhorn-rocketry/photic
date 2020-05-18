/**
 * Linear algebra matrix class with a few simple operations. More complex
 * operations can be found in MathUtils.hpp.
 *
 * NOTES:
 *
 *   (1) Arithmetic legality is enforced at compile time through templates.
 *
 *   (2) The template parameters defining matrix dimensions are of type Dim_t.
 *       Consequently, the largest supported matrix size is 255x255. This is
 *       plenty big for most applications.
 *
 *   (3) There is no index bounds checking in the interest of speed.
 */

#ifndef PHOTIC_MATRIX_HPP
#define PHOTIC_MATRIX_HPP

#include <cstring>

#include "Types.hpp"

/**
 * Number of elements in the matrix.
 */
#define ELEM_COUNT (T_Rows * T_Cols)

/**
 * Formula for the index of a matrix element in an array of matrix contiguous
 * matrix rows.
 *
 * @param   kRow Row index.
 * @param   kCol Column index.
 *
 * @ret     Index in array.
 */
#define ELEM_IDX(kRow, kCol) (T_Cols * kRow + kCol)

namespace Photic
{

template <Dim_t T_Rows, Dim_t T_Cols>
class Matrix final
{
public:
    /**
     * PUBLIC FOR USE BY UTILITIES ONLY -- DO NOT USE OUTSIDE THIS FILE
     *
     * Matrix elements stored contiguously by row.
     */
    Real_t mData[ELEM_COUNT];

    /**
     * Default constructor does nothing.
     *
     * WARNING: Matrix may be filled with garbage.
     */
    Matrix () {}

    /**
     * Constructor that fills the matrix with some value.
     *
     * @param   kFill Fill value.
     */
    Matrix (const Real_t kFill)
    {
        this->fill (kFill);
    }

    /**
     * Copy constructor.
     *
     * @param   kRhs RHS matrix.
     */
    Matrix (const Matrix<T_Rows, T_Cols>& kRhs)
    {
        memcpy (mData, kRhs.mData, sizeof (mData));
    }

    /**
     * Fills the matrix with some value.
     *
     * @param   kFill Fill value.
     */
    void fill (const Real_t kFill)
    {
        for (uint32_t i = 0; i < ELEM_COUNT; i++)
        {
            mData[i] = kFill;
        }
    }

    /**
     * Constant element access operator.
     *
     * @param   kRow Row index.
     * @param   kCol Column index.
     *
     * @ret     Element at (kRow, kCol).
     */
    Real_t operator() (const Dim_t kRow, const Dim_t kCol) const
    {
        return mData[ELEM_IDX (kRow, kCol)];
    }

    /**
     * Element access operator that allows mutation.
     *
     * @param   kRow Row index.
     * @param   kCol Column index.
     *
     * @ret     Reference to element at (kRow, kCol).
     */
    Real_t& operator() (const Dim_t kRow, const Dim_t kCol)
    {
        return mData[ELEM_IDX (kRow, kCol)];
    }

    /**
     * Vector constant element access operator.
     *
     * NOTE: Intended only for use with 1-column matrices.
     *
     * @param   kIdx Element index.
     *
     * @ret     kIdxth element in vector.
     */
    Real_t operator[] (const Dim_t kIdx) const
    {
        return mData[kIdx];
    }

    /**
     * Vector element access operator that allows mutation.
     *
     * NOTE: Intended only for use with 1-column matrices.
     *
     * @param   kIdx Element index.
     *
     * @ret     Reference to kIdxth element in vector.
     */
    Real_t& operator[] (const Dim_t kIdx)
    {
        return mData[kIdx];
    }

    /**
     * Computes this matrix plus another. Enables equations like a = b + c.
     *
     * @param   kRhs RHS matrix.
     *
     * @ret     This matrix plus the RHS.
     */
    Matrix<T_Rows, T_Cols> operator+ (const Matrix<T_Rows, T_Cols>& kRhs) const
    {
        Matrix<T_Rows, T_Cols> mat = *this;

        for (uint32_t i = 0; i < ELEM_COUNT; i++)
        {
            mat.mData[i] += kRhs.mData[i];
        }

        return mat;
    }

    /**
     * Computes this matrix minus another. Enables equations like a = b - c.
     *
     * @param   kRhs RHS matrix.
     *
     * @ret     This matrix minus the RHS.
     */
    Matrix<T_Rows, T_Cols> operator- (const Matrix<T_Rows, T_Cols>& kRhs) const
    {
        Matrix<T_Rows, T_Cols> mat = *this;

        for (uint32_t i = 0; i < ELEM_COUNT; i++)
        {
            mat.mData[i] -= kRhs.mData[i];
        }

        return mat;
    }

    /**
     * Computes the product of two matrices. Enables equations like a = b * c.
     *
     * NOTE: This uses the naive O(n^3) algorithm. The next best algorithm
     * (Strassen's) only becomes advantageous around n=100 or so. This is
     * technically within the bounds of Dim_t, but if you're multiplying 100x100
     * matrices in flight, I'd really like to hear wtf you're doing.
     *
     * @param   kRhs RHS matrix.
     *
     * @ret     This matrix times the RHS.
     */
    template <Dim_t T_RhsCols>
    Matrix<T_Rows, T_RhsCols> operator* (const Matrix<T_Cols, T_RhsCols>& kRhs) const
    {
        Matrix<T_Rows, T_RhsCols> mat;

        for (Dim_t i = 0; i < T_Rows; i++)
        {
            for (Dim_t j = 0; j < T_RhsCols; j++)
            {
                Real_t elem = 0;

                for (Dim_t k = 0; k < T_Cols; k++)
                {
                    elem += mData[ELEM_IDX (i, k)] * kRhs (k, j);
                }

                mat (i, j) = elem;
            }
        }

        return mat;
    }

    /**
     * Computes this matrix times a scalar.
     *
     * @param   kScalar Scalar.
     *
     * @ret     Scaled matrix.
     */
    template <typename T_Scalar>
    Matrix<T_Rows, T_Cols> operator* (const T_Scalar kScalar) const
    {
        Matrix<T_Rows, T_Cols> mat = *this;

        for (uint32_t i = 0; i < ELEM_COUNT; i++)
        {
            mat.mData[i] = (Real_t) (mat.mData[i] * kScalar);
        }

        return mat;
    }

    /**
     * Gets if two matrices are equal. Enables expressions like if (a == b).
     * Mostly used for testing.
     *
     * @param   kRhs RHS matrix.
     *
     * @ret     If this matrix and the RHS are equal.
     */
    bool operator== (const Matrix<T_Cols, T_Rows>& kRhs) const
    {
        // Types are the same, so can compare element buffers directly.
        return memcmp (mData, kRhs.mData, sizeof (mData)) == 0;
    }

    /**
     * Gets the transpose of this matrix.
     *
     * @ret     Transpose.
     */
    Matrix<T_Cols, T_Rows> transpose () const
    {
        Matrix<T_Cols, T_Rows> mat;

        for (Dim_t i = 0; i < T_Rows; i++)
        {
            for (Dim_t j = 0; j < T_Cols; j++)
            {
                mat (j, i) = mData[ELEM_IDX (i, j)];
            }
        }

        return mat;
    }
};

/**
 * Typedefs for common vector sizes.
 */
typedef Matrix<2, 1> Vector2_t;
typedef Matrix<3, 1> Vector3_t;
typedef Matrix<4, 1> Vector4_t;

} // namespace Photic

#endif