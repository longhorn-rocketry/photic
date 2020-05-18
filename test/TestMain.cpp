/**
 * Unit test suite entry point.
 */

#include "TestMatrix.hpp"
#include "TestMathUtils.hpp"
#include "TestKalmanFilter.hpp"
#include "TestIMUInterface.hpp"
#include "TestBarometerInterface.hpp"
#include "TestHistory.hpp"
#include "TestRocketTracker.hpp"

int main (int ac, char** av)
{
    SUITE_START;

    // Tests with no dependencies that should run on any platform.
    TestMatrix::test ();
    TestMathUtils::test ();
    TestIMUInterface::test ();
    TestBarometerInterface::test ();
    TestHistory::test ();

    // Tests that rely on specific STL components that may or may not be
    // available on the target platform.
    TestKalmanFilter::test ();
    TestRocketTracker::test ();

    SUITE_END;
}