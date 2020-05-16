#include <string.h>

#include "platform/barometer.h"

namespace photic
{

Barometer::Barometer()
{
    memset(&m_data, 0, sizeof m_data);
}

Barometer::Data_t Barometer::data()
{
    return m_data;
}

} // namespace photic
