/**
 * IMU device wrapper.
 */

#ifndef PHOTIC_PLATFORM_IMU_H
#define PHOTIC_PLATFORM_IMU_H

#include "math/matrix.h"
#include "platform/device.h"

namespace photic
{

class Imu : public Device
{
public:
    /**
     * Common IMU data.
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
        float gyro_x;
        float gyro_y;
        float gyro_z;

        /**
         * Quaternion orientation.
         */
        float quat_w;
        float quat_x;
        float quat_y;
        float quat_z;

        /**
         * Magnetometer readings.
         */
        float mag_x;
        float mag_y;
        float mag_z;

        /**
         * Some IMUs also measure temperature for some reason.
         */
        float temp;
    } Data_t;

    /**
     * Member data is initially zeroed-out.
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

    /**
     * Returns a 3x1 matrix with the last measured acceleration vector
     * <x, y, z>.
     *
     * @ret     Acceleration vector.
     */
    matrix accel();

    /**
     * Returns a 3x1 matrix with the last measured gyroscope angles <x, y, z>.
     *
     * @ret     Gyroscope angles.
     */
    matrix gyro();

    /**
     * Returns the last measured quaternion orientation.
     *
     * @ret     Quaternion orientation.
     */
    matrix quat();

    /**
     * Returns a 3x1 matrix with the last measured magnetic field vector
     * <x, y, z>.
     *
     * @ret     Magnetic field vector.
     */
    matrix mag();

    /**
     * Returns the last measured temperature.
     *
     * @ret     Temperature.
     */
    float temp();

protected:
    /**
     * Most recent sensor readings.
     */
    Data_t m_data;
};

} // namespace photic

#endif
