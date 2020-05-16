/**
 * Barometer device wrapper.
 */

#ifndef PHOTIC_PLATFORM_BAROMETER_H
#define PHOTIC_PLATFORM_BAROMETER_H

#include "platform/device.h"

namespace photic
{

class Barometer : public Device
{
public:
    /**
     * Common barometer measurements.
     */
    typedef struct Data
    {
        /**
         * Air pressure and temperature.
         */
        float pressure;
        float temperature;

        /**
         * Some barometers also estimate altitude based on their other readings.
         */
        float altitude;
    } Data_t;

    /**
     * Zeros out m_data.
     */
    Barometer();

    /**
     * Initializes and establishes a connection with the barometer.
     *
     * @ret     True if successful, false otherwise;
     */
    virtual bool init() = 0;

    /**
     * Reads the most recent sensor readings into m_data.
     *
     *
     * @ret     True if successful, false otherwise.
     */
    virtual bool update() = 0;

    /**
     * Returns a copy of m_data, which holds IMU readings as of the last update
     * call.
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
