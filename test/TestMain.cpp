#include <stdio.h>

#include "TestMatrix.hpp"
#include "TestKalmanFilter.hpp"
#include "TestIMUInterface.hpp"
#include "TestBarometerInterface.hpp"

int main (int ac, char** av)
{
    SUITE_START;

    TestMatrix::test ();
    TestKalmanFilter::test ();
    TestIMUInterface::test ();
    TestBarometerInterface::test ();

    SUITE_END;
}