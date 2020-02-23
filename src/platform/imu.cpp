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

matrix Imu::accel()
{
    return photic::matrix(3, 1, m_data.accel_x, m_data.accel_y, m_data.accel_z);
}

matrix Imu::gyro()
{
    return photic::matrix(3, 1, m_data.gyro_x, m_data.gyro_y, m_data.gyro_z);
}

matrix Imu::mag()
{
    return photic::matrix(3, 1, m_data.mag_x, m_data.mag_y, m_data.mag_z);
}

float Imu::temp()
{
    return m_data.temp;
}

} // namespace photic
