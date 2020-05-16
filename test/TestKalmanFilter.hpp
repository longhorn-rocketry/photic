/**
 * Tests for KalmanFilter.
 */

#include <ctime>
#include <math.h>
#include <random>

#ifndef TEST_KALMAN_FILTER_HPP
#define TEST_KALMAN_FILTER_HPP

#include "KalmanFilter.hpp"
#include "TestMacros.hpp"

using namespace Photic;

namespace TestKalmanFilter
{

/**
 * Tests that the Kalman filter improves estimation accuracy.
 *
 * This test runs a simple 1-DOF simulation of an object falling in Earth
 * gravity with no other forces acting on it. Observations of the object's
 * position and velocity are masked by normal error distributions. The state
 * of the object is tracked for 100 seconds with both dead reckoning and a
 * Kalman filter. At the end of the test, the filtered state is verified to be
 * more accurate than the dead reckoned state, and the filtered state is
 * verified to be within a certain percent error of the true state.
 *
 * NOTE: Because this test is partially based on pseudorandom numbers, it may
 * fail. As long as it fails very, very rarely, the Kalman filter is working
 * satisfactorily.
 */
void test ()
{
    TEST_DEFINE ("KalmanFilterAccuracyIncrease");

    // Current timestep, timestep size, and duration of simulation.
    Real_t t = 0;
    const Real_t tStep = 0.1;
    const Real_t duration = 100;

    // Variance in position and acceleration observations.
    const Real_t posVariance = 15.45;
    const Real_t accelVariance = 1.8;

    // Error distributions for position and acceleration observations.
    std::mt19937 generator (std::time (0));
    std::normal_distribution<Real_t> posErrDistr (0, sqrt (posVariance));
    std::normal_distribution<Real_t> accelErrDistr (0, sqrt (accelVariance));

    // Kalman filter configured to match above data.
    KalmanFilter kf;
    kf.setDeltaT (tStep);
    kf.setSensorVariance (posVariance, accelVariance);
    kf.setInitialState (0, 0, 0);
    kf.computeKg (100);

    // True state of the system.
    Vector3_t stateTrue (0);
    // System state estimated by dead reckoning.
    Vector3_t stateDeadreck (0);
    // System state estimate by Kalman filter.
    Vector3_t stateFiltered (0);

    while (t < duration)
    {
        // Extrapolate true state.
        stateTrue[2] = 9.81;
        stateTrue[1] += stateTrue[2] * tStep;
        stateTrue[0] += stateTrue[1] * tStep;

        // Observe true state by applying errors.
        Real_t posObserved = stateTrue[0] + posErrDistr (generator);
        Real_t accelObserved = stateTrue[2] + accelErrDistr (generator);

        // Update dead reckoned estimate.
        stateDeadreck[2] = accelObserved;
        stateDeadreck[1] += stateDeadreck[2] * tStep;
        stateDeadreck[0] += stateDeadreck[1] * tStep;

        // Update filtered estimate.
        stateFiltered = kf.filter (posObserved, accelObserved);

        // Advance time.
        t += tStep;
    }

    // Compute the error in the dead reckoned and filtered estimates.
    Vector3_t errorDeadreck = stateTrue - stateDeadreck;
    Vector3_t errorFiltered = stateTrue - stateFiltered;

    Real_t errorDeadreckPos   = fabs (errorDeadreck[0]);
    Real_t errorDeadreckVel   = fabs (errorDeadreck[1]);
    Real_t errorDeadreckAccel = fabs (errorDeadreck[2]);

    Real_t errorFilteredPos   = fabs (errorFiltered[0]);
    Real_t errorFilteredVel   = fabs (errorFiltered[1]);
    Real_t errorFilteredAccel = fabs (errorFiltered[2]);

    // Check that filter error is less than dead reckon error.
    CHECK_TRUE (errorFilteredPos   < errorDeadreckPos  );
    CHECK_TRUE (errorFilteredVel   < errorDeadreckVel  );
    CHECK_TRUE (errorFilteredAccel < errorDeadreckAccel);

    // Check that filtered state is very close to the true state. The acceptable
    // error bound is somewhat generous since this simulation is ideal.
    Real_t posPercentError =   errorFilteredPos   / stateTrue[0];
    Real_t velPercentError =   errorFilteredVel   / stateTrue[1];
    Real_t accelPercentError = errorFilteredAccel / stateTrue[2];
    CHECK_TRUE (posPercentError   < 0.01);
    CHECK_TRUE (velPercentError   < 0.01);
    CHECK_TRUE (accelPercentError < 0.01);
}

} // namespace TestKalmanFilter

#endif