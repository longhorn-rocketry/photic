#ifndef PHOTONIC_MATH_MATRIX_H
#define PHOTONIC_MATH_MATRIX_H

namespace photic {

/**
 * A lightweight matrix implementation designed for speed. The internal storage
 * is of staic size Matrix::m_MATRIX_STATIC_SIZE, which can be increased as
 * needed. We choose a default value of 3 because 3x3 is the largest size
 * required by photic::KalmanFilter.
 *
 * Note that, in the interest of speed, there is no bounds checking whatsoever.
 * There are also no exceptions for illegal matrix arithmetic in the interest of
 * reducing STL dependencies. If bad operands are broadcast together, a 0x0
 * matrix is returned, which will produce more 0x0 matrices in subsequent
 * computations. This effect is similar to NaN flooding.
 */
class Matrix {
protected:
  static const int m_MATRIX_STATIC_SIZE = 3;
  float m_mat[m_MATRIX_STATIC_SIZE][m_MATRIX_STATIC_SIZE];
  int m_rows, m_cols;

public:
  /**
   * @brief Default constructor creates a 0x0 "null" matrix.
   */
  Matrix();

  /**
   * Creates a matrix and populates it with values.
   *
   * @param   k_rows   row count
   * @param   k_cols   column count
   * @param   k_v0...8 values to insert into the matrix from left to right, top
   *                   to bottom
   */
  Matrix(int k_rows, int k_cols,
         float k_v0=0, float k_v1=0, float k_v2=0,
         float k_v3=0, float k_v4=0, float k_v5=0,
         float k_v6=0, float k_v7=0, float k_v8=0);

  /**
   * @brief Copy constructor.
   */
  Matrix(const Matrix &k_copy);

  /**
   * @brief Fills the matrix left to right, top to bottom from an array.
   */
  void fill(const float k_contents[]);

  /**
   * @brief Gets the row count.
   */
  int rows() const;

  /**
   * @brief Gets the column count.
   */
  int cols() const;

  /**
   * @brief Allows access via bracket indexing.
   */
  float* operator[](int k_i);

  /**
   * @brief Allows mutation via parentheses indexing.
   */
  float& operator()(int k_r, int k_c);

  /**
   * Constant access.
   *
   * @param   k_r row index
   * @param   k_c column index
   *
   * @ret     value at row k_r and column k_c
   */
  float get(int k_r, int k_c) const;

  /**
   * @brief Adds two matrices together.
   */
  friend Matrix operator+(const Matrix& k_a, const Matrix& k_b);

  /**
   * @brief Subtracts two matrices.
   */
  friend Matrix operator-(const Matrix& k_a, const Matrix& k_b);

  /**
   * @brief Multiplies two matrices.
   */
  friend Matrix operator*(const Matrix& k_a, const Matrix& k_b);

  /**
   * @brief Computes the inverse of this matrix as though it were 2x2.
   */
  Matrix inv2x2() const;

  /**
   * @brief Computes the inverse of this matrix as though it were 3x3.
   */
  Matrix inv3x3() const;

  /**
   * @brief Computes the transposition of this matrix.
   */
  Matrix t() const;
};

typedef Matrix matrix;

extern const matrix NULLMAT;

} // end namespace photic

#endif
