/**
 * RocketTracker combines many of Photic's components into one compact
 * navigation utility. It may be used directly in flight software or simply
 * taken as an example of the integrated usage of other objects in the library.
 *
 * RocketTracker uses the following components:
 *
 *   - IMUInterface and BarometerInterface for communicating with the rocket's
 *     sensors.
 *   - History for analyzing variance in the rocket's sensor readings.
 *   - KalmanFilter (and therefore Matrix and much of MathUtils) for filtering
 *     sensor noise and accurately tracking the rocket's state.
 *
 *                              ---- USAGE ----
 *
 *   (1) Implement an IMUInterface and BarometerInterface to communicate with
 *       your rocket's IMU and barometer (see IMUInterface.hpp and
 *       BarometerInterface.hpp for details).
 *
 *   (2) Grab the default RocketTracker config from RocketTracker::
 *       getDefaultConfig and set the following configuration parameters:
 *
 *         pImu         Pointer to rocket IMU interface.
 *         pBarometer   Pointer to rocket barometer interface.
 *         vertAccelIdx 0, 1, or 2; the component of IMU acceleration vectors
 *                      corresponding to the vertical direction.
 *
 *       Setting (or at least being informed of) the other configuration
 *       parameters is recommended but not necessary (see RocketTracker::
 *       getDefaultConfig for details).
 *
 *   (3) Create a RocketTracker by providing the config to the constructor.
 *
 *         RocketTracker tracker (config);
 *
 *       RocketTracker will profile the behavior of your rocket's sensors as
 *       part of this step. This may take a minute or so depending on sensor
 *       communication speed.
 *
 *       Any and all disturbance (e.g. vibration, wind) to the flight
 *       computer should be minimized during this time. The flight computer
 *       should lie flat on a table, in the open air, with its vertical axis
 *       pointing into the sky.
 *
 *   (4) Call RocketTracker::track every iteration of the rocket's flight logic
 *       loop. It should be called at a rate corresponding to the timestep size
 *       passed to the config (every 0.1 seconds by default). This function
 *       returns the estimated altitude, vertical velocity, and vertical
 *       acceleration of the rocket.
 *
 *         Vector3_t rocketState = tracker.track ();
 *
 *       Or, if you're running your sensor interfaces elsewhere:
 *
 *         imu->run ();
 *         barometer->run ();
 *         
 *         ...
 *         
 *         Vector3_t rocketState = tracker.track (false);
 */

#ifndef PHOTIC_ROCKET_TRACKER_HPP
#define PHOTIC_ROCKET_TRACKER_HPP

#include "KalmanFilter.hpp"
#include "IMUInterface.hpp"
#include "BarometerInterface.hpp"

namespace Photic
{

class RocketTracker final
{
public:
    /**
     * RocketTracker configuration parameters.
     */
    typedef struct
    {
        IMUInterface* pImu;             /* Rocket IMU interface. */
        BarometerInterface* pBarometer; /* Rocket barometer interface. */
        Real_t dt;                      /* Tracker timestep. */
        Dim_t vertAccelIdx;             /* Accel vector idx w/ vertical comp. */
        uint32_t kgIterations;          /* Kalman gain calc iterations. */
    } Config_t;

    /**
     * Gets a default config with several recommended config values. Some values
     * do still need to be set by the user. The config is as follows:
     *
     *   pImu         = nullptr The user must provide this.
     *   pBarometer   = nullptr The user must provide this.
     *   dt           = 0.1     This is the smallest recommended timestep.
     *   vertAccelIdx = 2       This is the index standardized by Adafruit.
     *                          Industry prefers 0. This depends on the IMU
     *                          interface implementation.
     *   kgIterations = 50      This is based on LRA's experience, but the
     *                          optimal value ultimately depends on sensors
     *                          and thrust. See note (2) in KalmanFilter.hpp for
     *                          more details.
     *
     * @ret     Default configuration.
     */
    static Config_t getDefaultConfig ();

    /**
     * Configures the RocketTracker.
     *
     * NOTE: RocketTracker profiles sensor behavior as part of this function.
     * See usage step (3) above for details.
     *
     * @param   kConfig Config.
     */
    RocketTracker (const Config_t& kConfig);

    /**
     * Gets the altitude, vertical velocity, and vertical acceleration of the
     * rocket.
     *
     * NOTE: This function must be called at a rate with timestep size
     * corresponding to the dt specified in the config.
     *
     * @param   kRunSensors Whether or not to run the IMU and barometer
     *                      interfaces to get their most recent readings.
     *                      Defaults to true.
     *
     * @ret     Estimated altitude, vertical velocity, and vertical acceleration
     *          of the rocket.
     */
    Vector3_t track (const bool kRunSensors = true);

private:
    IMUInterface* mPImu;             /* Rocket IMU interface. */
    BarometerInterface* mPBarometer; /* Rocket barometer interface. */
    const Dim_t mVertAccelIdx;       /* Accel vector idx w/ vertical comp. */
    KalmanFilter mKf;                /* Tracking Kalman filter. */

    /**
     * Estimates the variance in altitude and acceleration readings by
     * analyzing sensor readings over a period of time. Estimates the launchpad
     * altitude based on the average altitude measurement seen during this time.
     *
     * @param   kBaroVarRet Variance in barometer altitude measurements.
     * @param   kImuVarRet  Variance in IMU vertical acceleration measurements.
     * @param   kLpAltRet   Estimated launchpad altitude.
     */
    void profileSensors (Real_t& kBaroVarRet, Real_t& kImuVarRet,
                         Real_t& kLpAltRet) const;
};

} // namespace Photic

#endif