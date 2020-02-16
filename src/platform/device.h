/**
 * Abstraction for a device within the flight computer system. Sensors,
 * actuators, and controllers are all devices.
 */
#ifndef PHOTIC_PLATFORM_DEVICE_H
#define PHOTIC_PLATFORM_DEVICE_H

namespace photic
{

class Device
{
public:
    /**
     * One-time setup logic.
     *
     * @ret     True if successful, false otherwise.
     */
    virtual bool init() = 0;

    /**
     * Iterative logic.
     *
     * @ret     True if successful, false otherwise.
     */
    virtual bool update() = 0;
};

} // namespace photic

#endif
