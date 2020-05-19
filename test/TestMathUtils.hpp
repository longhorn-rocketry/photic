/**
 * Tests for MathUtils.
 */

#ifndef TEST_MATH_UTILS_HPP
#define TEST_MATH_UTILS_HPP

#include "MathUtils.hpp"
#include "TestMacros.hpp"

namespace TestMathUtils
{

/**
 * Tests the matrix construction utilities.
 */
void testMathUtilsMatrixConstruction ()
{
    TEST_DEFINE ("MathUtilsMatrixConstruction");

    Matrix<3, 3> mat0 = MathUtils::makeMatrix3 (1, 2, 3,
                                                4, 5, 6,
                                                7, 8, 9);
    CHECK_EQUAL (mat0 (0, 0), 1);
    CHECK_EQUAL (mat0 (0, 1), 2);
    CHECK_EQUAL (mat0 (0, 2), 3);
    CHECK_EQUAL (mat0 (1, 0), 4);
    CHECK_EQUAL (mat0 (1, 1), 5);
    CHECK_EQUAL (mat0 (1, 2), 6);
    CHECK_EQUAL (mat0 (2, 0), 7);
    CHECK_EQUAL (mat0 (2, 1), 8);
    CHECK_EQUAL (mat0 (2, 2), 9);

    Matrix<2, 2> mat1 = MathUtils::makeMatrix2 (10, 11,
                                                12, 13);
    CHECK_EQUAL (mat1 (0, 0), 10);
    CHECK_EQUAL (mat1 (0, 1), 11);
    CHECK_EQUAL (mat1 (1, 0), 12);
    CHECK_EQUAL (mat1 (1, 1), 13);                                       
}

/**
 * Tests vector construction, access, and mutation.
 */
void testMathVectorConstructAccessMutate ()
{
    TEST_DEFINE ("MathUtilsVectorConstructAccessMutate");

    // Make a 2-vector and check contents.
    Vector2_t vec0 = MathUtils::makeVector2 (1, 2);
    CHECK_EQUAL (vec0[0], 1);
    CHECK_EQUAL (vec0[1], 2);

    // Change elements and recheck contents.
    vec0[0] = 3;
    vec0[1] = 4;
    CHECK_EQUAL (vec0[0], 3);
    CHECK_EQUAL (vec0[1], 4);

    // Make a 3-vector and check contents.
    Vector3_t vec1 = MathUtils::makeVector3 (1, 2, 3);
    CHECK_EQUAL (vec1[0], 1);
    CHECK_EQUAL (vec1[1], 2);
    CHECK_EQUAL (vec1[2], 3);

    // Change elements and recheck contents.
    vec0[0] = 4;
    vec0[1] = 5;
    vec0[2] = 6;
    CHECK_EQUAL (vec0[0], 4);
    CHECK_EQUAL (vec0[1], 5);
    CHECK_EQUAL (vec0[2], 6);

    // Make a 4-vector and check contents.
    Vector4_t vec2 = MathUtils::makeVector4 (1, 2, 3, 4);
    CHECK_EQUAL (vec2[0], 1);
    CHECK_EQUAL (vec2[1], 2);
    CHECK_EQUAL (vec2[2], 3);
    CHECK_EQUAL (vec2[3], 4);

    // Change elements and recheck contents.
    vec2[0] = 5;
    vec2[1] = 6;
    vec2[2] = 7;
    vec2[3] = 8;
    CHECK_EQUAL (vec2[0], 5);
    CHECK_EQUAL (vec2[1], 6);
    CHECK_EQUAL (vec2[2], 7);
    CHECK_EQUAL (vec2[3], 8);
}

/**
 * Tests inverting a 2x2 matrix.
 */
void testMathUtilsMatrixInvertMatrix2 ()
{
    TEST_DEFINE ("MathUtilsMatrixInvertMatrix2");

    Matrix<2, 2> mat0 = MathUtils::makeMatrix2 ( 1, 11,
                                                -7, 25);
    Matrix<2, 2> mat1 = MathUtils::makeMatrix2 (25.0 / 102, -11.0 / 102,
                                                 7.0 / 102,   1.0 / 102);
    Matrix<2, 2> mat2 = MathUtils::invertMatrix2 (mat0);
    CHECK_TRUE (mat2 == mat1);
}

/**
 * Tests 3-vector cross products.
 */
void testMathUtilsCrossProduct ()
{
    TEST_DEFINE ("MathUtilsCrossProduct");

    Vector3_t vec0 = MathUtils::makeVector3 (1, 2, 3);
    Vector3_t vec1 = MathUtils::makeVector3 (0, 0, 0);
    Vector3_t vec2 = MathUtils::cross (vec0, vec1);
    CHECK_EQUAL (vec2[0], 0);
    CHECK_EQUAL (vec2[1], 0);
    CHECK_EQUAL (vec2[2], 0);

    vec0 = MathUtils::makeVector3 (-1.5, 0.25,  9.76);
    vec1 = MathUtils::makeVector3 (34.6, 8.102, 6   );
    vec2 = MathUtils::cross (vec0, vec1);
    CHECK_APPROX (vec2[0],  -77.57552, 1e-3);
    CHECK_APPROX (vec2[1],  346.696,   1e-3);
    CHECK_APPROX (vec2[2],  -20.803,   1e-3);
}

/**
 * Tests vector-quaternion rotation.
 */
void testMathUtilsRotateVector ()
{
    TEST_DEFINE ("MathUtilsRotateVector");

    // These test cases are randomly generated. Answers verified with Eigen.
    Vector4_t quat   = MathUtils::makeVector4 (0.6252, -0.1941,  0.5203,  0.5485);
    Vector3_t vec    = MathUtils::makeVector3 (0.8233, -0.6049, -0.3296);
    Vector3_t result = MathUtils::makeVector3 (0.2751, -0.0651, -1.0356);

    Vector3_t vecRot = MathUtils::rotateVector (quat, vec);
    CHECK_APPROX (result[0], vecRot[0], 1e-3);
    CHECK_APPROX (result[1], vecRot[1], 1e-3);
    CHECK_APPROX (result[2], vecRot[2], 1e-3);

    quat   = MathUtils::makeVector4 (0.7594, -0.6292,  0.1528, -0.0640);
    vec    = MathUtils::makeVector3 (0.2577, -0.2704,  0.0268);
    result = MathUtils::makeVector3 (0.2777, -0.1036,  0.2290);

    vecRot = MathUtils::rotateVector (quat, vec);
    CHECK_APPROX (result[0], vecRot[0], 1e-3);
    CHECK_APPROX (result[1], vecRot[1], 1e-3);
    CHECK_APPROX (result[2], vecRot[2], 1e-3);

    quat   = MathUtils::makeVector4 (0.6792,  0.6251,  0.2038,  0.3263);
    vec    = MathUtils::makeVector3 (-0.7168,  0.2139, -0.9674);
    result = MathUtils::makeVector3 (-1.2074,  0.1935, -0.0150);

    vecRot = MathUtils::rotateVector (quat, vec);
    CHECK_APPROX (result[0], vecRot[0], 1e-3);
    CHECK_APPROX (result[1], vecRot[1], 1e-3);
    CHECK_APPROX (result[2], vecRot[2], 1e-3);
}

void test ()
{
    testMathUtilsMatrixConstruction ();
    testMathVectorConstructAccessMutate ();
    testMathUtilsMatrixInvertMatrix2 ();
    testMathUtilsCrossProduct ();
    testMathUtilsRotateVector ();
}

} // namespace TestMathUtils

#endif