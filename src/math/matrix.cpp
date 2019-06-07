#include "math/matrix.h"

const matrix NULLMAT(0, 0);

Matrix::Matrix() {
  _rows = 0;
  _cols = 0;
}

Matrix::Matrix(int rows, int cols, float v0, float v1, float v2, float v3,
               float v4, float v5, float v6, float v7, float v8) {
  _rows = rows;
  _cols = cols;
  float contents[] = {v0, v1, v2, v3, v4, v5, v6, v7, v8};
  fill(contents);
}

Matrix::Matrix(const Matrix &copy) {
  _rows = copy._rows;
  _cols = copy._cols;
  for (int r = 0; r < _rows; r++)
    for (int c = 0; c < _cols; c++)
      mat[r][c] = copy.get(r, c);
}

void Matrix::fill(const float contents[]) {
  for (int r = 0; r < _rows; r++)
    for (int c = 0; c < _cols; c++)
      mat[r][c] = contents[r * _cols + c];
}

int Matrix::rows() const {
  return _rows;
}

int Matrix::cols() const {
  return _cols;
}

std::ostream& operator<<(std::ostream &stream, const Matrix &mat) {
  for (int r = 0; r < mat._rows; r++) {
    for (int c = 0; c < mat._cols; c++) {
      stream << mat.get(r, c) << " ";
    }
    stream << "\n";
  }
  return stream;
}

float* Matrix::operator[](int i) {
  return mat[i];
}

float& Matrix::operator()(int r, int c) {
  return mat[r][c];
}

float Matrix::get(int r, int c) const {
  return mat[r][c];
}

Matrix operator+(const Matrix &a, const Matrix&b) {
  if (a._rows != b._rows || a._cols != b._cols)
    return NULLMAT;

  matrix sum(a._rows, a._cols);
  for (int r = 0; r < sum._rows; r++)
    for (int c = 0; c < sum._cols; c++)
      sum[r][c] = a.get(r, c) + b.get(r, c);

  return sum;
}

Matrix operator-(const Matrix &a, const Matrix&b) {
  if (a._rows != b._rows || a._cols != b._cols)
    return NULLMAT;

  matrix diff(a._rows, a._cols);
  for (int r = 0; r < diff._rows; r++)
    for (int c = 0; c < diff._cols; c++)
      diff[r][c] = a.get(r, c) - b.get(r, c);

  return diff;
}

Matrix operator*(const Matrix &a, const Matrix &b) {
  if (a._cols != b._rows)
    return NULLMAT;

  matrix prod(a._rows, b._cols);
  for (int r = 0; r < prod._rows; r++)
    for (int c = 0; c < prod._cols; c++) {
      float k = 0;
      for (int j = 0; j < a._cols; j++)
        k += a.get(r, j) * b.get(j, c);
      prod[r][c] = k;
    }

  return prod;
}

Matrix Matrix::inv2x2() const {
  float det = mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
  return Matrix(2, 2,
      mat[1][1] / det, -mat[0][1] / det,
      -mat[1][0] / det, mat[0][0] / det);
}

Matrix Matrix::inv3x3() const {
  float det = mat[0][0] * (mat[1][1] * mat[2][2] - mat[2][1] * mat[1][2]) -
              mat[0][1] * (mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0]) +
              mat[0][2] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);

  return Matrix(3, 3,
      (mat[1][1] * mat[2][2] - mat[2][1] * mat[1][2]) / det,
      (mat[0][2] * mat[2][1] - mat[0][1] * mat[2][2]) / det,
      (mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1]) / det,
      (mat[1][2] * mat[2][0] - mat[1][0] * mat[2][2]) / det,
      (mat[0][0] * mat[2][2] - mat[0][2] * mat[2][0]) / det,
      (mat[1][0] * mat[0][2] - mat[0][0] * mat[1][2]) / det,
      (mat[1][0] * mat[2][1] - mat[2][0] * mat[1][1]) / det,
      (mat[2][0] * mat[0][1] - mat[0][0] * mat[2][1]) / det,
      (mat[0][0] * mat[1][1] - mat[1][0] * mat[0][1]) / det);
}

Matrix Matrix::t() const {
  Matrix trans(_cols, _rows);

  for (int r = 0; r < _rows; r++)
    for (int c = 0; c < _cols; c++)
      trans[c][r] = mat[r][c];

  return trans;
}
