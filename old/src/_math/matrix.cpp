#include <math.h>

#include "math/matrix.h"
#include "math/rocketmath.h"

namespace photic
{

const matrix NULLMAT(0, 0);

Matrix::Matrix()
{
    m_rows = 0;
    m_cols = 0;
}

Matrix::Matrix(std::size_t k_rows, std::size_t k_cols,
               float k_v0, float k_v1, float k_v2,
               float k_v3, float k_v4, float k_v5,
               float k_v6, float k_v7, float k_v8)
{
    m_rows = k_rows;
    m_cols = k_cols;
    float contents[] = {k_v0, k_v1, k_v2, k_v3, k_v4, k_v5, k_v6, k_v7, k_v8};
    fill(contents);
}

Matrix::Matrix(const Matrix &k_copy)
{
  m_rows = k_copy.m_rows;
  m_cols = k_copy.m_cols;

  for (std::size_t r = 0; r < m_rows; r++)
  {
      for (std::size_t c = 0; c < m_cols; c++)
      {
          m_mat[r][c] = k_copy.get(r, c);
      }
  }

}

void Matrix::fill(const float k_contents[])
{
    for (std::size_t r = 0; r < m_rows; r++)
    {
        for (std::size_t c = 0; c < m_cols; c++)
        {
            m_mat[r][c] = k_contents[r * m_cols + c];
        }
    }
}

std::size_t Matrix::rows() const
{
    return m_rows;
}

std::size_t Matrix::cols() const
{
    return m_cols;
}

float* Matrix::operator[](std::size_t k_i)
{
    return m_mat[k_i];
}

float& Matrix::operator()(std::size_t k_r, std::size_t k_c)
{
    return m_mat[k_r][k_c];
}

float Matrix::get(std::size_t k_r, std::size_t k_c) const
{
    return m_mat[k_r][k_c];
}

Matrix operator+(const Matrix& k_a, const Matrix& k_b)
{
    if (k_a.m_rows != k_b.m_rows || k_a.m_cols != k_b.m_cols)
    {
        return NULLMAT;
    }

    matrix sum(k_a.m_rows, k_a.m_cols);

    for (std::size_t r = 0; r < sum.m_rows; r++)
    {
        for (std::size_t c = 0; c < sum.m_cols; c++)
        {
            sum[r][c] = k_a.get(r, c) + k_b.get(r, c);
        }
    }

    return sum;
}

Matrix operator-(const Matrix& k_a, const Matrix& k_b)
{
    if (k_a.m_rows != k_b.m_rows || k_a.m_cols != k_b.m_cols)
    {
        return NULLMAT;
    }

    matrix diff(k_a.m_rows, k_a.m_cols);

    for (std::size_t r = 0; r < diff.m_rows; r++)
    {
        for (std::size_t c = 0; c < diff.m_cols; c++)
        {
            diff[r][c] = k_a.get(r, c) - k_b.get(r, c);
        }
    }

    return diff;
}

Matrix operator*(const Matrix& k_a, const Matrix& k_b)
{
    if (k_a.m_cols != k_b.m_rows)
    {
        return NULLMAT;
    }

    matrix prod(k_a.m_rows, k_b.m_cols);

    for (std::size_t r = 0; r < prod.m_rows; r++)
    {
        for (std::size_t c = 0; c < prod.m_cols; c++)
        {
            float k = 0;

            for (std::size_t j = 0; j < k_a.m_cols; j++)
            {
                k += k_a.get(r, j) * k_b.get(j, c);
            }

            prod[r][c] = k;
        }
    }

    return prod;
}

bool operator==(const Matrix& k_a, const Matrix& k_b)
{
    if (k_a.m_rows != k_b.m_rows || k_a.m_cols != k_b.m_cols)
    {
        return false;
    }

    for (std::size_t r = 0; r < k_a.m_rows; r++)
    {
        for (std::size_t c = 0; c < k_a.m_cols; c++)
        {
            if (k_a.get(r, c) != k_b.get(r, c))
            {
                return false;
            }
        }
    }

    return true;
}

bool operator!=(const Matrix& k_a, const Matrix& k_b)
{
    return !(k_a == k_b);
}

Matrix Matrix::inv() const
{
    if (m_rows == 2 && m_cols == 2)
    {
        float det = m_mat[0][0] * m_mat[1][1] - m_mat[0][1] * m_mat[1][0];
        return matrix(2, 2,
            m_mat[1][1] / det, -m_mat[0][1] / det,
            -m_mat[1][0] / det, m_mat[0][0] / det
        );
    }
    else if (m_rows == 3 && m_cols == 3)
    {
        float det =
            m_mat[0][0] * (m_mat[1][1] * m_mat[2][2] - m_mat[2][1] * m_mat[1][2]) -
            m_mat[0][1] * (m_mat[1][0] * m_mat[2][2] - m_mat[1][2] * m_mat[2][0]) +
            m_mat[0][2] * (m_mat[1][0] * m_mat[2][1] - m_mat[1][1] * m_mat[2][0]);

        return matrix(3, 3,
            (m_mat[1][1] * m_mat[2][2] - m_mat[2][1] * m_mat[1][2]) / det,
            (m_mat[0][2] * m_mat[2][1] - m_mat[0][1] * m_mat[2][2]) / det,
            (m_mat[0][1] * m_mat[1][2] - m_mat[0][2] * m_mat[1][1]) / det,
            (m_mat[1][2] * m_mat[2][0] - m_mat[1][0] * m_mat[2][2]) / det,
            (m_mat[0][0] * m_mat[2][2] - m_mat[0][2] * m_mat[2][0]) / det,
            (m_mat[1][0] * m_mat[0][2] - m_mat[0][0] * m_mat[1][2]) / det,
            (m_mat[1][0] * m_mat[2][1] - m_mat[2][0] * m_mat[1][1]) / det,
            (m_mat[2][0] * m_mat[0][1] - m_mat[0][0] * m_mat[2][1]) / det,
            (m_mat[0][0] * m_mat[1][1] - m_mat[1][0] * m_mat[0][1]) / det
        );
    }

    return NULLMAT;
}

Matrix Matrix::t() const
{
    Matrix trans(m_cols, m_rows);

    for (std::size_t r = 0; r < m_rows; r++)
    {
        for (std::size_t c = 0; c < m_cols; c++)
        {
            trans[c][r] = m_mat[r][c];
        }
    }

    return trans;
}

bool Matrix::approx(const Matrix& k_a, const Matrix& k_b)
{
    if (k_a.rows() != k_b.rows() || k_a.cols() != k_b.cols())
    {
        return false;
    }

    for (std::size_t r = 0; r < k_a.rows(); r++)
    {
        for (std::size_t c = 0; c < k_a.cols(); c++)
        {
            if (!fapprox(k_a.get(r, c), k_b.get(r, c)))
            {
                return false;
            }
        }
    }

    return true;
}

} // namespace photic
