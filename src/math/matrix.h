#ifndef PHOTONIC_MATH_MATRIX_H
#define PHOTONIC_MATH_MATRIX_H

#include <iostream>

/**
  A lightweight matrix implementation designed for speed. The internal storage
  is of static size MATRIX_STATIC_SIZE, which can be increased as needed. We
  chose a default value of 3, as 3x3 is the largest size required by Photonic's
  Kalman filter.

  Note that, in the interest of speed, there is no bounds checking whatsoever.

  To reduce STL dependencies, rather than throw exceptions for illegal matrix
  arithmetic, the computation will simply return a 0x0 matrix that will produce
  0x0 matrices in all subsequent computations. This effect is similar to NaN
  flooding. If your computations are yielding empty matrices, you are
  broadcasting incompatible matrices together somewhere.
*/
class Matrix {
protected:
  static const int MATRIX_STATIC_SIZE = 3;
  float mat[MATRIX_STATIC_SIZE][MATRIX_STATIC_SIZE];
  int _rows, _cols;

public:
  /**
    @brief default constructor; a 0x0 "null" matrix
  */
  Matrix();

  /**
    Creates a matrix and populates it with values.

    @param rows row count
    @param cols column count
    @param v0...v8 values inserted into the matrix from left to right, top to
           bottom
  */
  Matrix(int rows, int cols,
         float v0=0, float v1=0, float v2=0,
         float v3=0, float v4=0, float v5=0,
         float v6=0, float v7=0, float v8=0);

  /**
    @brief copy constructor
  */
  Matrix(const Matrix &copy);

  /**
    @brief fills the matrix from left to right, top to bottom with values from
           an array
  */
  void fill(const float contents[]);

  /**
    @brief gets the row count
  */
  int rows() const;

  /**
    @brief gets the column count
  */
  int cols() const;

  /**
    @brief allows inserting into streams; purely for debug and doesn't compute
           ASCII column widths
  */
  friend std::ostream& operator<<(std::ostream &stream, const Matrix &mat);

  /**
    @brief allows matrix set and get with bracket indexing
  */
  float* operator[](int i);

  /**
    @brief allows matrix set and get with parentheses indexing
  */
  float& operator()(int r, int c);

  /**
    @brief gets the number at position rc
  */
  float get(int r, int c) const;

  /**
    @brief adds two matrices together
  */
  friend Matrix operator+(const Matrix &a, const Matrix &b);

  /**
    @brief subtracts two matrices
  */
  friend Matrix operator-(const Matrix &a, const Matrix &b);

  /**
    @brief multiplies two matrices
  */
  friend Matrix operator*(const Matrix &a, const Matrix &b);

  /**
    @brief computes the inverse of this matrix as though it were 2x2
  */
  Matrix inv2x2() const;

  /**
    @brief computes the inverse of this matrix as though it were 3x3
  */
  Matrix inv3x3() const;

  /**
    @brief computes the transposition of this matrix
  */
  Matrix t() const;
};

typedef Matrix matrix;

extern const matrix NULLMAT;

#endif
