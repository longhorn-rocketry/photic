/**
 *                                 [PHOTIC]
 *                                  v3.2.0
 *
 * This file is part of Photic, a collection of utilities for writing high-power
 * rocket flight computer software. Developed in Austin, TX by the Longhorn
 * Rocketry Association at the University of Texas at Austin.
 */

#include "BarometerInterface.hpp"

namespace Photic
{

BarometerInterface::BarometerInterface () {}

Real_t BarometerInterface::getPressure ()
{
    return mData.pressure;
}

Real_t BarometerInterface::getTemperature ()
{
    return mData.temperature;
}

Real_t BarometerInterface::getAltitude ()
{
    return mData.altitude;
}

} // namespace Photic