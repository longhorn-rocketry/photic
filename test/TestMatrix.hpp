/**
 * Tests for Matrix.
 */

#ifndef TEST_MATRIX_HPP
#define TEST_MATRIX_HPP

#include "Matrix.hpp"
#include "MathUtils.hpp"
#include "TestMacros.hpp"

using namespace Photic;

namespace TestMatrix
{

/**
 * Tests matrix construction, access, and mutation.
 */
void testMatrixConstructAccessMutate ()
{
    TEST_DEFINE ("MatrixConstructAccessMutate");

    // Check that fill constructor correctly fills matrix.
    Matrix<2, 2> mat0 (3);
    CHECK_EQUAL (mat0 (0, 0), 3);
    CHECK_EQUAL (mat0 (1, 0), 3);
    CHECK_EQUAL (mat0 (0, 1), 3);
    CHECK_EQUAL (mat0 (1, 1), 3);

    // Check that matrix can be refilled.
    mat0.fill (5);
    CHECK_EQUAL (mat0 (0, 0), 5);
    CHECK_EQUAL (mat0 (1, 0), 5);
    CHECK_EQUAL (mat0 (0, 1), 5);
    CHECK_EQUAL (mat0 (1, 1), 5);

    // Check mutation.
    mat0(0, 0) = 8;
    mat0(1, 0) = 8;
    mat0(0, 1) = 8;
    mat0(1, 1) = 8;
    CHECK_EQUAL (mat0 (0, 0), 8);
    CHECK_EQUAL (mat0 (1, 0), 8);
    CHECK_EQUAL (mat0 (0, 1), 8);
    CHECK_EQUAL (mat0 (1, 1), 8);

    // Check constant access.
    const Matrix<2, 2> mat1 (3);
    CHECK_EQUAL (mat1 (0, 0), 3);
    CHECK_EQUAL (mat1 (1, 0), 3);
    CHECK_EQUAL (mat1 (0, 1), 3);
    CHECK_EQUAL (mat1 (1, 1), 3);
}

/**
 * Tests the matrix addition operation.
 */
void testMatrixAddition ()
{
    TEST_DEFINE ("MatrixAddition");

    // Check that a matrix plus its negation is the zero matrix. All elements in
    // this matrix are unique, so this is a thorough enough exercise of the
    // addition algorithm.
    Matrix<3, 3> mat0 = MathUtils::makeMatrix3 (1, 2, 3,
                                                4, 5, 7,
                                                7, 8, 9);
    Matrix<3, 3> mat1 = MathUtils::makeMatrix3 (-1, -2, -3,
                                                -4, -5, -7,
                                                -7, -8, -9);
    Matrix<3, 3> mat2 = mat0 + mat1;
    Matrix<3, 3> mat3 (0);
    CHECK_TRUE (mat2 == mat3);
}

/**
 * Tests the matrix subtraction operation.
 */
void testMatrixSubtraction ()
{
    TEST_DEFINE ("MatrixSubtraction");

    // Check that a matrix plus its negation is the zero matrix. All elements in
    // this matrix are unique, so this is a thorough enough exercise of the
    // subtraction algorithm.
    Matrix<3, 3> mat0 = MathUtils::makeMatrix3 (1, 2, 3,
                                                4, 5, 7,
                                                7, 8, 9);
    Matrix<3, 3> mat1 = mat0;
    Matrix<3, 3> mat2 = mat0 - mat1;
    Matrix<3, 3> mat3 (0);
    CHECK_TRUE (mat2 == mat3);
}

/**
 * Tests the matrix multiplication operation.
 */
void testMatrixMultiplication ()
{
    TEST_DEFINE ("MatrixMultiplication");

    // Compute a simple product with distinct 3x3 matrices. In industry,
    // writing just this test case and calling it a day would be egregious. I'm
    // confident enough in the implementation to cut this corner. Plus, if
    // matrix multiplication inexplicably breaks for matrices of other sizes,
    // this will almost certainly break the Kalman filter, tested elsewhere. 
    Matrix<3, 3> mat0 = MathUtils::makeMatrix3 (1, 2, 3,
                                                4, 5, 6,
                                                7, 8, 9);
    Matrix<3, 3> mat1 = MathUtils::makeMatrix3 (-5,  0, 10,
                                                 2, -4, 53,
                                                 1,  1, 7);
    Matrix<3, 3> mat2 = mat0 * mat1;
    Matrix<3, 3> mat3 = MathUtils::makeMatrix3 (  2,  -5, 137,
                                                 -4, -14, 347,
                                                -10, -23, 557);
    CHECK_TRUE (mat2 == mat3);
}

/**
 * Tests multiplying a matrix by a scalar.
 */
void testMatrixScalarMultiplication ()
{
    TEST_DEFINE ("MatrixScalarMultiplication");

    Matrix<2, 2> mat0 = MathUtils::makeMatrix2 (1, 2,
                                                3, 4);
    Matrix<2, 2> mat1 = MathUtils::makeMatrix2 ( -4,  -8,
                                                -12, -16);
    Matrix<2, 2> mat2 = mat0 * -4;
    CHECK_TRUE (mat1 == mat2);
}

/**
 * Tests matrix comparison with ==.
 */
void testMatrixEquation ()
{
    TEST_DEFINE ("MatrixEquation");

    // Build identical matrices and verify they are equal.
    Matrix<3, 3> mat0 = MathUtils::makeMatrix3 (1, 2, 3,
                                                4, 5, 6,
                                                7, 8, 9);
    Matrix<3, 3> mat1 = MathUtils::makeMatrix3 (1, 2, 3,
                                                4, 5, 6,
                                                7, 8, 9);
    CHECK_TRUE (mat0 == mat1);

    // Zero each element of the LHS individually and check that this
    // causes it to become unequal to the RHS.
    for (uint32_t i = 0; i < 9; i++)
    {
        Matrix<3, 3> mat2 = mat1;
        mat2.mData[i] = 0;
        CHECK_TRUE (!(mat2 == mat1));
    }
}

/**
 * Tests matrix transposition.
 */
void testMatrixTranspose ()
{
    TEST_DEFINE ("MatrixTranspose");

    // Another egregiously weak test case. Sue me.
    Matrix<2, 3> mat0;
    mat0 (0, 0) = 1;
    mat0 (0, 1) = 2;
    mat0 (0, 2) = 3;
    mat0 (1, 0) = 4;
    mat0 (1, 1) = 5;
    mat0 (1, 2) = 6;

    Matrix<3, 2> mat1 = mat0.transpose ();
    CHECK_EQUAL (mat1 (0, 0), 1);
    CHECK_EQUAL (mat1 (0, 1), 4);
    CHECK_EQUAL (mat1 (1, 0), 2);
    CHECK_EQUAL (mat1 (1, 1), 5);
    CHECK_EQUAL (mat1 (2, 0), 3);
    CHECK_EQUAL (mat1 (2, 1), 6);
}

/**
 * Entry point for matrix tests.
 */
void test ()
{
    testMatrixConstructAccessMutate ();
    testMatrixAddition ();
    testMatrixSubtraction ();
    testMatrixEquation ();
    testMatrixMultiplication ();
    testMatrixScalarMultiplication ();
    testMatrixTranspose ();
}

} // namespace TestMatrix

#endif