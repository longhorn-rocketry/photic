/**
 * Tests for RocketTracker.
 */

#ifndef TEST_ROCKET_TRACKER_HPP
#define TEST_ROCKET_TRACKER_HPP

#include <ctime>
#include <math.h>
#include <random>

#include "RocketTracker.hpp"
#include "TestMacros.hpp"

using namespace Photic;

namespace TestRocketTracker
{

/**
 * True state of the simulated rocket.
 */
Vector3_t stateTrue (0);

/**
 * Variance in altitude and acceleration sensor readings.
 */
const Real_t posVariance = 15.45;
const Real_t accelVariance = 1.8;

/**
 * Error distributions for altitude and acceleration sensor readings.
 */
std::mt19937 generator (std::time (0));
std::normal_distribution<Real_t> posErrDistr (0, sqrt (posVariance));
std::normal_distribution<Real_t> accelErrDistr (0, sqrt (accelVariance));

/**
 * BarometerInterface which pulls readings from the state globals set in the
 * simulation loop.
 */
class SimulationBarometerInterface final : public BarometerInterface
{
public:
    SimulationBarometerInterface () {}

    virtual bool init ()
    {
        return true;
    }

    virtual bool run ()
    {
        mData.altitude = stateTrue[0] + posErrDistr (generator);

        return true;
    }
};

/**
 * IMUInterface which pulls readings from the state globals set in the
 * simulation loop.
 */
class SimulationIMUInterface final : public IMUInterface
{
public:
    SimulationIMUInterface () {}

    virtual bool init ()
    {
        return true;
    }

    virtual bool run ()
    {
        mData.vecAccel[0] = accelErrDistr (generator);
        mData.vecAccel[1] = accelErrDistr (generator);
        mData.vecAccel[2] = stateTrue[2] + accelErrDistr (generator);

        // Unit quaternion; rocket remains perfectly upright throughout
        // simulation.
        mData.orientQuat[0] = 1;
        mData.orientQuat[1] = 0;
        mData.orientQuat[2] = 0;
        mData.orientQuat[3] = 0;

        return true;
    }
};

/**
 * Tests that RocketTracker correctly tracks the rocket's state. This runs the
 * same falling simulation and accuracy tests as KalmanFilterAccuracyIncrease in
 * TestKalmanFilter.hpp.
 */
void test ()
{
    TEST_DEFINE ("RocketTracker");

    // Current timestep, timestep size, and duration of simulation.
    Real_t t = 0;
    const Real_t tStep = 0.1;
    const Real_t duration = 100;

    // Create sensor interfaces.
    IMUInterface* pImu = new SimulationIMUInterface ();
    BarometerInterface* pBarometer = new SimulationBarometerInterface ();

    // Configure RocketTracker to interface with simulation.
    RocketTracker::Config_t trackerConfig = RocketTracker::getDefaultConfig ();
    trackerConfig.pImu = pImu;
    trackerConfig.pBarometer = pBarometer;
    trackerConfig.kgIterations = 100;

    RocketTracker tracker (trackerConfig);

    // Rocket state estimate by RocketTracker.
    Vector3_t stateTracked (0);

    // Run falling simulation loop.
    while (t < duration)
    {
        // Extrapolate true rocket state.
        stateTrue[2] = 9.81;
        stateTrue[1] += stateTrue[2] * tStep;
        stateTrue[0] += stateTrue[1] * tStep;

        // Update tracked state.
        stateTracked = tracker.track ();

        // Advance time.
        t += tStep;
    }

    // Compute the error in the tracked state.
    Vector3_t errorTracked  = stateTrue - stateTracked;
    Real_t errorTrackedPos   = fabs (errorTracked[0]);
    Real_t errorTrackedVel   = fabs (errorTracked[1]);
    Real_t errorTrackedAccel = fabs (errorTracked[2]);

    // Check that tracker state is very close to the true state. The acceptable
    // error bound is somewhat generous since this simulation is ideal.
    Real_t posPercentError =   errorTrackedPos   / stateTrue[0];
    Real_t velPercentError =   errorTrackedVel   / stateTrue[1];
    Real_t accelPercentError = errorTrackedAccel / stateTrue[2];
    CHECK_TRUE (posPercentError   < 0.01);
    CHECK_TRUE (velPercentError   < 0.01);
    CHECK_TRUE (accelPercentError < 0.01);

    delete pImu;
    delete pBarometer;
}

} // namespace RocketTrackerTests

#endif