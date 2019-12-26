/**
 * IMU device wrapper.
 */

#ifndef PHOTIC_PLATFORM_IMU_H
#define PHOTIC_PLATFORM_IMU_H

#include "device.h"

namespace photic
{

class Imu : public Device
{
public:
    /**
     * The common 9 IMU DoFs.
     */
    typedef struct Data
    {
        /**
         * Accelerometer readings.
         */
        float accel_x;
        float accel_y;
        float accel_z;

        /**
         * Gyroscope readings.
         */
        float gyro_r;
        float gyro_p;
        float gyro_y;

        /**
         * Magnetometer readings.
         */
        float mag_x;
        float mag_y;
        float mag_z;
    } Data_t;

    /**
     * Zeros out m_data.
     */
    Imu();

    /**
     * Initializes and establishes a connection with the IMU.
     *
     * @ret     True if successful, false otherwise.
     */
    virtual bool init() = 0;

    /**
     * Reads the most recent sensor readings into m_data.
     *
     * @ret     True if successful, false otherwise.
     */
    virtual bool update() = 0;

    /**
     * Returns a copy of m_data, which holds IMU readings as of the last
     * update() call.
     *
     * @ret     Most recent sensor readings.
     */
    Data_t data();

protected:
    /**
     * Most recent sensor readings.
     */
    Data_t m_data;
};

} // namespace photic

#endif
