#include "TestMatrix.hpp"
#include "TestKalmanFilter.hpp"
#include "TestIMUInterface.hpp"
#include "TestBarometerInterface.hpp"
#include "TestHistory.hpp"

int main (int ac, char** av)
{
    SUITE_START;

    // Tests with no dependencies that should run on any platform.
    TestMatrix::test ();
    TestIMUInterface::test ();
    TestBarometerInterface::test ();
    TestHistory::test ();

    // Tests that rely on specific STL components that may or may not be
    // available on the target platform.
    TestKalmanFilter::test ();

    SUITE_END;
}