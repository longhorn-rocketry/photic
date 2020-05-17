/**
 * 1-DOF Kalman filter designed for high power rocketry applications. Barometric
 * altitude estimate and vertical acceleration reading go in, filtered state
 * <altitude, velocity, acceleration> comes out.
 *
 * Based on "Digital Detection of Rocket Apogee" by Dougal, Kwok, and Luckett:
 * http://cnx.org/content/col11599/1.1/
 *
 *                              ---- USAGE ----
 *
 *   Each of the following instructions are performed once in setup code unless
 *   otherwise specified.
 *
 *   (1) Construct a KalmanFilter. Use KalmanFilter::setDeltaT to set the
 *       size of the timesteps. This is usually the period of the flight logic
 *       loop in which the filtering is done.
 *
 *   (2) Set the altimeter and accelerometer sensor variance with
 *       KalmanFilter::setSensorVariance. These values can be estimated by
 *       allowing each sensor to rest flat on a table and computing the variance
 *       in their readings over a period of time.
 *
 *       It is best practice to compute this on the launch site for maximum
 *       accuracy. Building this computation into the flight computer's startup
 *       sequence is even better.
 *
 *       Take care when calculating the accelerometer variance. An acceleration
 *       observation supplied to the filter is only the vertical component of
 *       acceleration. Accordingly, the accelerometer variance should be only
 *       the variance in a single component of the measured acceleration vector
 *       and not the entire vector.
 *
 *   (3) Set the rocket's initial state with KalmanFilter::setInitialState.
 *       This is usually <0, 0, 0> or <launchpad altitude, 0, 0> depending on
 *       your preferred frame.
 *
 *   (4) Use KalmanFilter::computeKg (uint32_t) to compute the Kalman gain with
 *       some number of iterations. As mentioned in the notes, this number may
 *       need to be determined experimentally. We usually do between 25 and 50
 *       iterations, but this will ultimately depend on your sensors and thrust.
 *
 *   (5) This step is performed every iteration of the flight logic loop.
 *
 *       Take the altitude measurement from the barometer and the vertical
 *       acceleration reading from the IMU and pass them to
 *       KalmanFilter::filter. A Vector3_t comprised of <altitude, velocity,
 *       acceleration> is returned.
 *
 *       Vertical acceleration readings must be relative to the Earth and not
 *       relative to the rocket (otherwise the filter is tracking the arc length
 *       of the rocket's flight and not vertical displacement from the
 *       launchpad). Most IMUs produce acceleration readings relative to the
 *       rocket. To get acceleration relative to the Earth, rotate the
 *       acceleration vector by the IMU's measured orientation. This requires
 *       that the IMU be initialized with its vertical axis parallel to the
 *       launch rail, i.e. pointing into the sky.
 *
 *       For example, using the Adafruit BNO055 IMU, this process looks
 *       something like this:
 *
 *         // Read rocket orientation and acceleration vector.
 *         imu::Quaternion orientation = bno.getQuat ();
 *         sensor_event_t event;
 *         bno.getEvent (&event, Adafruit_BNO055::VECTOR_LINEARACCEL);
 *
 *         // Rotate acceleration vector into the world frame.
 *         imu::Vector<3> rocketAccel (event.acceleration.x,
 *                                     event.acceleration.y,
 *                                     event.acceleration.z);
 *         imu::Vector<3> worldAccel = orientation.rotateVector (rocketAccel);
 *
 *         ...
 *
 *         // Filter the state (assuming Z is the vertical direction).
 *         Vector3_t rocketState = kf.filter (altitude, worldAccel[2]);
 *
 *                              ---- NOTES ----
 *
 * This filter has been used extensively in simulated and real high power
 * flights. Some observations of its behavior and performance:
 *
 *   (1) The filter responds poorly to dramatic changes in the update vector,
 *       likely because the covariance matrix does not change. This is most
 *       notable during periods of high jerk, e.g. liftoff and motor burnout.
 *       Following burnout, it may take the filter several seconds to drive
 *       the error back to (near) zero.
 *
 *   (2) The rate at which error is driven to zero is greatly dependent on the
 *       number of iterations in the Kalman gain calculation. This number may
 *       need to be tuned somewhat. The corollary to this is that filter
 *       accuracy does not approach perfect as the number of iterations in the
 *       Kalman gain calculation approaches infinity. There is a balance to be
 *       struck.
 *
 *   (3) If there is enough disagreement between the position and acceleration
 *       observations, NaNs may appear in filter estimates. This usually
 *       requires that something very strange happen with a sensor, e.g. inert
 *       air around the barometer is suddenly accelerated into the sensor face
 *       on liftoff, temporarily causing very low altitude readings. This can be
 *       fixed with simple sanity checks on the observations being passed to
 *       the filter, e.g. floor altitude observations at the launchpad altitude.
 */

#ifndef PHOTIC_KALMAN_FILTER_HPP
#define PHOTIC_KALMAN_FILTER_HPP

#include "Matrix.hpp"
#include "Types.hpp"

namespace Photic
{

class KalmanFilter final
{
public:
    /**
     * Filter is uninitialized and filled with garbage. See usage instructions
     * above.
     */
    KalmanFilter ();

    /**
     * Sets the timestep size for filter iterations.
     *
     * @param   kDt Timestep size.
     */
    void setDeltaT (const Time_t kDt);

    /**
     * Sets the variance in altitude and acceleration readings.
     *
     * @param   kAltVar   Altitude reading variance.
     * @param   kAccelVar Acceleration reading variance.
     */
    void setSensorVariance (const Real_t kAltVar, const Real_t kAccelVar);

    /**
     * Sets the rocket's initial state.
     *
     * @param   kAlt   Initial altitude.
     * @param   kVel   Initial velocity.
     * @param   kAccel Initial acceleration.
     */
    void setInitialState (const Real_t kAlt, const Real_t kVel,
                          const Real_t kAccel);

    /**
     * Computes the Kalman gain.
     *
     * @param   kIterations Number of iterations in calculation.
     */
    void computeKg (const uint32_t kIterations);

    /**
     * Advances the filter and returns a new state estimate.
     *
     * @param   kAlt   Current altitude reading.
     * @param   kAccel Current acceleration reading.
     *
     * @ret     Estimated state <altitude, velocity, acceleration>.
     */
    Vector3_t filter (const Real_t kAlt, const Real_t kAccel);

private:
    Matrix<3, 3> mA; /* State transition matrix. */
    Matrix<3, 3> mQ; /* Process noise covariance. Unused currently. */
    Matrix<2, 3> mH; /* Mapping of state to observations. */
    Matrix<2, 2> mR; /* Measurement noise covariance. */
    Matrix<3, 3> mP; /* Error covariance. */
    Matrix<3, 2> mK; /* Kalman gain. */
    Matrix<3, 1> mE; /* Last computed state estimate. */

    /**
     * Performs a single refinement on the current Kalman gain based on the
     * current error covariance. Called iteratively by computeKg (uint32_t).
     */
    void computeKg ();
};

} // namespace Photic

#endif