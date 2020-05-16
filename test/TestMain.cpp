#include <stdio.h>

#include "TestMatrix.hpp"
#include "TestKalmanFilter.hpp"

int main (int ac, char** av)
{
    SUITE_START;

    TestMatrix::test ();
    TestKalmanFilter::test ();

    SUITE_END;
}