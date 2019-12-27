#include <string.h>

#include "platform/imu.h"

namespace photic
{

Imu::Imu()
{
    memset(&m_data, 0, sizeof m_data);
}

Imu::Data_t Imu::data()
{
    return m_data;
}

} // namespace photic
