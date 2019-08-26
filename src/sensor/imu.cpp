#include "imu.h"

namespace photic {

void Imu::read(ImuData& k_dest) {
	k_dest.ax = m_data.ax;
	k_dest.ay = m_data.ay;
	k_dest.az = m_data.az;

	k_dest.gx = m_data.gx;
	k_dest.gy = m_data.gy;
	k_dest.gz = m_data.gz;

	k_dest.mx = m_data.mx;
	k_dest.my = m_data.my;
	k_dest.mz = m_data.mz;
}

float Imu::get_acc_x() {
	return m_data.ax;
}

float Imu::get_acc_y() {
	return m_data.ay;
}

float Imu::get_acc_z() {
	return m_data.az;
}

float Imu::get_gyro_x() {
	return m_data.gx;
}

float Imu::get_gyro_y() {
	return m_data.gx;
}

float Imu::get_gyro_z() {
	return m_data.gz;
}

float Imu::get_mag_x() {
	return m_data.mx;
}

float Imu::get_mag_y() {
	return m_data.my;
}


float Imu::get_mag_z() {
	return m_data.mz;
}

} // end namespace photic
