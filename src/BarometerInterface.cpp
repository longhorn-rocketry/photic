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