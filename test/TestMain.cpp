#include <stdio.h>

#include "TestMatrix.hpp"
#include "TestKalmanFilter.hpp"
#include "TestIMUInterface.hpp"

int main (int ac, char** av)
{
    SUITE_START;

    TestMatrix::test ();
    TestKalmanFilter::test ();
    TestIMUInterface::test ();

    SUITE_END;
}