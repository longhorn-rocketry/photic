/**
 *                                 [PHOTIC]
 *                                  v3.2.0
 *
 * This file is part of Photic, a collection of utilities for writing high-power
 * rocket flight computer software. Developed in Austin, TX by the Longhorn
 * Rocketry Association at the University of Texas at Austin.
 *
 *                            ---- THIS FILE ----
 *
 * An interface for writing barometer device wrappers. Photic provides these
 * interfaces so that sensor drivers can be switched between easily, e.g. to
 * interface flight software with a hardware-out-of-the-loop simulation.
 *
 *                              ---- USAGE ----
 *
 *   (1) Extend BarometerInterface into a child class that will act as the
 *       driver for your specific IMU, e.g.
 *
 *         class BMP085Barometer final : public Photic::BarometerInterface
 *
 *   (2) Implement the init function. This function performs one-time setup
 *       logic, e.g. connecting to the sensor and setting calibration values.
 *
 *   (3) Implement run function. This function should read the most recent
 *       barometer measurements and place them in the appropriate members within
 *       BarometerInterface::mData.
 *
 *   (4) In your flight software, call init once and then run periodically in
 *       the flight logic loop. After each run, access the necessary readings
 *       with the getXXX functions.
 */

#ifndef PHOTIC_BAROMETER_INTERFACE_HPP
#define PHOTIC_BAROMETER_INTERFACE_HPP

#include "Types.hpp"

namespace Photic
{

class BarometerInterface
{
public:
    /**
     * Common barometer readings.
     */
    typedef struct
    {
        Real_t pressure;
        Real_t temperature;
        Real_t altitude;
    } Data_t;

    /**
     * NOTE: Barometer data is initially filled with garbage.
     */
    BarometerInterface ();

    /**
     * Performs one-time setup logic for the barometer.
     *
     * @ret     If setup succeeded.
     */
    virtual bool init () = 0;

    /**
     * Collects the most recent measurements from the barometer.
     *
     * @ret     If run succeeded.
     */
    virtual bool run () = 0;

    /**
     * Gets the most recent pressure reading.
     *
     * @ret     Pressure reading.
     */
    Real_t getPressure ();

    /**
     * Gets the most recent temperature reading.
     *
     * @ret     Temperature reading.
     */
    Real_t getTemperature ();

    /**
     * Gets the most recent altitude reading.
     *
     * @ret     Acceleration vector.
     */
    Real_t getAltitude ();

protected:
    /**
     * Most recent data read from barometer.
     */
    Data_t mData;
};

} // namespace Photic

#endif