#include "matrix.h"
#include "testutil.h"

#include <CppUTest/TestHarness.h>

using namespace photic;

TEST_GROUP(MatrixTest)
{

};

/**
 * Matrices are constructed, accessed, and mutated correctly.
 */
TEST(MatrixTest, ConstructAccessMutate)
{
    matrix a(2, 2,
        1, 0,
        0, 1
    );

    CHECK_APPROX(1, a[0][0]);
    CHECK_APPROX(0, a[1][0]);
    CHECK_APPROX(0, a[0][1]);
    CHECK_APPROX(1, a[1][1]);

    a[0][0] = 3.14;
    a[0][1] = 2.71;
    a[1][0] = 1.61;
    a[1][1] = 4.67;

    CHECK_APPROX(3.14, a[0][0]);
    CHECK_APPROX(1.61, a[1][0]);
    CHECK_APPROX(2.71, a[0][1]);
    CHECK_APPROX(4.67, a[1][1]);

    a(0, 0) = 9.81;
    a(0, 1) = 1.41;
    a(1, 0) = 2.23;
    a(1, 1) = 4.89;

    CHECK_APPROX(9.81, a[0][0]);
    CHECK_APPROX(2.23, a[1][0]);
    CHECK_APPROX(1.41, a[0][1]);
    CHECK_APPROX(4.89, a[1][1]);
}

/**
 * Matrix equation is correct.
 */
TEST(MatrixTest, Equate)
{
    matrix a(3, 3,
        1, 2, 3,
        1, 0, 1,
        3, 4, 9
    );

    matrix b(3, 3,
        1, 2, 3,
        1, 0, 1,
        3, 4, 9
    );

    CHECK_TRUE(a == b);
    CHECK_TRUE(matrix::approx(a, b));

    b = matrix(3, 3,
        1, 2, 3,
        1, 1, 1,
        3, 4, 9
    );

    CHECK_TRUE(a != b);
    CHECK_TRUE(!matrix::approx(a, b));

    b = matrix(2, 3,
        1, 2, 3,
        1, 1, 1
    );

    CHECK_TRUE(a != b);

    a = NULLMAT;
    b = matrix();

    CHECK_TRUE(a == b);
    CHECK_TRUE(matrix::approx(a, b));

    a = matrix(2, 2,
        1 / 3.0, 0.15,
        3.14159265359, 0.000000001
    );

    b = matrix(2, 2,
        0.3333333333, 0.6 / 4,
        3.1415926535, 0.0000000013
    );

    CHECK_TRUE(a != b);
    CHECK_TRUE(matrix::approx(a, b));
}

/**
 * Matrix arithmetic is correct and produces NULLMAT when illegal.
 */
TEST(MatrixTest, Arithmetic)
{
    // Legal arithmetic.
    matrix a(2, 2,
        1, 1,
        1, 1
    );

    matrix b(2, 2,
        2, 2,
        2, 2
    );

    matrix c(2, 2,
        3, 3,
        3, 3
    );

    matrix d(2, 2,
        -1, -1,
        -1, -1
    );

    CHECK_TRUE(a + b == c);
    CHECK_TRUE(b + a == c);
    CHECK_TRUE(a - b == d);
    CHECK_TRUE(b - a == a);

    // Illegal arithmetic.
    a = matrix(3, 2,
        1, 2,
        3, 4,
        5, 6
    );

    CHECK_TRUE(a + b == NULLMAT);
    CHECK_TRUE(b + a == NULLMAT);
    CHECK_TRUE(a - b == NULLMAT);
    CHECK_TRUE(b - a == NULLMAT);
}

/**
 * Matrix multiplication is correct and produces NULLMAT when illegal.
 */
TEST(MatrixTest, Multiply)
{
    // Legal multiplication.
    matrix a(2, 2,
        3, 4,
        7, 8
    );

    matrix b(2, 2,
        1, 2,
        2, 0
    );

    matrix c(2, 2,
        11, 6,
        23, 14
    );

    CHECK_TRUE(a * b == c);

    matrix d(2, 1,
        5,
        6
    );

    matrix e(1, 2,
        -3, 1
    );

    matrix f(2, 2,
        -15, 5,
        -18, 6
    );

    CHECK_TRUE(d * e == f);

    // Illegal multiplication.
    matrix g(3, 3,
        9, 9, 9,
        9, 9, 9,
        9, 9, 9
    );

    CHECK_TRUE(a * g == NULLMAT);
    CHECK_TRUE(d * g == NULLMAT);
    CHECK_TRUE(e * g == NULLMAT);
    CHECK_TRUE(a * e == NULLMAT);
    CHECK_TRUE(a * NULLMAT == NULLMAT);
    CHECK_TRUE(NULLMAT * NULLMAT == NULLMAT);
}

/**
 * Matrices are inverted correctly.
 */
TEST(MatrixTest, Invert)
{
    matrix a(2, 2,
        11, 4,
        3, -7
    );

    matrix b(2, 2,
        -7.0 / -89, -4.0 / -89,
        -3.0 / -89, 11.0 / -89
    );

    matrix c = a.inv();

    CHECK_TRUE(matrix::approx(b, c));

    matrix d(3, 3,
        1, 2, -1,
        0, 6, 5,
        8, 8, -3
    );

    matrix e(3, 3,
        -29.0 / 35, -1.0 / 35,  8.0 / 35,
          4.0 /  7,  1.0 / 14, -1.0 / 14,
        -24.0 / 35,  4.0 / 35,  3.0 / 35
    );

    matrix f = d.inv();

    CHECK_TRUE(matrix::approx(e, f));

    matrix g = matrix(3, 3,
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    );

    CHECK_TRUE(g == g.inv());

    CHECK_TRUE(NULLMAT == NULLMAT.inv());
}

/**
 * Matrices are transposed correctly.
 */
TEST(MatrixTest, Transpose)
{
    matrix a(2, 1,
        3,
        4
    );

    matrix b(1, 2,
        3, 4
    );

    CHECK_TRUE(b == a.t());

    matrix c(2, 2,
        1, 2,
        3, 4
    );

    matrix d(2, 2,
        1, 3,
        2, 4
    );

    CHECK_TRUE(d == c.t());

    matrix e(3, 2,
        9, 8,
        7, 6,
        5, 4
    );

    matrix f(2, 3,
        9, 7, 5,
        8, 6, 4
    );

    CHECK_TRUE(f == e.t());

    matrix g(1, 1,
        9
    );

    matrix h(1, 1,
        9
    );

    CHECK_TRUE(h == g.t());

    CHECK_TRUE(NULLMAT == NULLMAT.t());
}
