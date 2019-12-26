/**
 * A lightweight matrix implementation designed for speed. The internal storage
 * array is of static size PHOTIC_MATRIX_MAX_DIMENSION, which can be increased
 * as needed. We choose a default value of 3 because 3x3 is the largest size
 * required by photic::KalmanFilter.
 *
 * In the interest of speed, there is no bounds checking whatsoever. There are
 * also no exceptions for illegal matrix arithmetic in the interest of reducing
 * STL dependencies. Illegal matrix arithmetic (e.g. adding matrices of
 * different sizes) will produce a 0x0 "null" matrix. This matrix will produce
 * more 0x0 matrices in subsequent computations, an effect similar to NaN
 * annihilation.
 */

#ifndef PHOTIC_MATH_MATRIX_H
#define PHOTIC_MATH_MATRIX_H

#define PHOTIC_MATRIX_MAX_DIMENSION 3

#include <stdint.h>
#include <vector>

namespace photic
{

class Matrix
{
public:
    /**
     * Creates a 0x0 "null" matrix.
     */
    Matrix();

    /**
     * Creates a matrix and populates it with values.
     *
     * @param   k_rows   Row count.
     * @param   k_cols   Column count.
     * @param   k_v0...8 Values to insert into the matrix from left to right,
     *                   top to bottom.
     */
    Matrix(std::size_t k_rows, std::size_t k_cols,
           float k_v0=0, float k_v1=0, float k_v2=0,
           float k_v3=0, float k_v4=0, float k_v5=0,
           float k_v6=0, float k_v7=0, float k_v8=0);

    /**
     * Copy constructor.
     *
     * @param   k_copy Matrix to copy.
     */
    Matrix(const Matrix &k_copy);

    /**
     * Fills the matrix left to right, top to bottom from an array.
     *
     * @param   k_contents Matrix entries.
     */
    void fill(const float k_contents[]);

    /**
     * Gets the row count.
     *
     * @ret     Row count.
     */
    std::size_t rows() const;

    /**
     * Gets the column count.
     *
     * @ret     Column count.
     */
    std::size_t cols() const;

    /**
     * Allows access and mutation via bracket indexing.
     *
     * @param   k_i Index.
     *
     * @ret     Row at index k_i.
     */
    float* operator[](std::size_t k_i);

    /**
     * Allows access and mutation via parentheses indexing.
     *
     * @param   k_r Row index.
     * @param   k_c Column index.
     *
     * @ret     Entry at index k_c of the k_rth row.
     */
    float& operator()(std::size_t k_r, std::size_t k_c);

    /**
     * Constant access.
     *
     * @param   k_r Row index.
     * @param   k_c Column index.
     *
     * @ret     Entry at index k_c of the k_rth row.
     */
    float get(std::size_t k_r, std::size_t k_c) const;

    /**
     * Adds two matrices.
     *
     * @param   k_a RHS matrix.
     * @param   k_b LHS matrix.
     *
     * @ret     k_a + k_b, or photic::NULLMAT if illegal.
     */
    friend Matrix operator+(const Matrix& k_a, const Matrix& k_b);

    /**
     * Subtracts two matrices.
     *
     * @param   k_a RHS matrix.
     * @param   k_b LHS matrix.
     *
     * @ret     k_a - k_b, or photic::NULLMAT if illegal.
     */
    friend Matrix operator-(const Matrix& k_a, const Matrix& k_b);

    /**
     * Multiplies two matrices.
     *
     * @param   k_a RHS matrix.
     * @param   k_b LHS matrix.
     *
     * @ret     k_a * k_b, or photic::NULLMAT if illegal.
     */
    friend Matrix operator*(const Matrix& k_a, const Matrix& k_b);

    /**
     * Equates two matrices.
     *
     * @param   k_a RHS matrix.
     * @param   k_b LHS matrix.
     *
     * @ret     True if same size and contents, false otherwise.
     */
    friend bool operator==(const Matrix& k_a, const Matrix& k_b);

    /**
     * Equates two matrices.
     *
     * @param   k_a RHS matrix.
     * @param   k_b LHS matrix.
     *
     * @ret     False if same size and contents, true otherwise.
     */
    friend bool operator!=(const Matrix& k_a, const Matrix& k_b);

    /**
     * Computes the inverse of this matrix. Supported inverse sizes:
     *
     *   - 2x2
     *   - 3x3
     *
     * @ret     Inverse of this matrix, or photic::NULLMAT if not supported.
     */
    Matrix inv() const;

    /**
     * Computes the transpose of this matrix.
     *
     * @ret     Transpose of this matrix.
     */
    Matrix t() const;

    /**
     * Gets if two matrices are approximately equal.
     *
     * @param   k_a RHS matrix.
     * @param   k_b LHS matrix.
     *
     * @ret     True if same size and corresponding entries with absolute
     *          differences less than or equal to 1e-6, false otherwise.
     */
    static bool approx(const Matrix& k_a, const Matrix& k_b);

protected:
    /**
     * Matrix contents.
     */
    float m_mat[PHOTIC_MATRIX_MAX_DIMENSION][PHOTIC_MATRIX_MAX_DIMENSION];
    /**
     * Number of rows.
     */
    std::size_t m_rows;
    /**
     * Number of cols.
     */
    std::size_t m_cols;
};

/**
 * We like lowercase data structures.
 */
typedef Matrix matrix;

/**
 * 0x0 "null" matrix produced by invalid matrix arithmetic.
 */
extern const matrix NULLMAT;

} // namespace photic

#endif
