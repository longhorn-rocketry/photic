#include "sensor/imu.h"

using namespace photonic;

void Imu::read(ImuData *dest) {
	dest->ax = data.ax;
	dest->ay = data.ay;
	dest->az = data.az;

	dest->gx = data.gx;
	dest->gy = data.gy;
	dest->gz = data.gz;

	dest->mx = data.mx;
	dest->my = data.my;
	dest->mz = data.mz;
}

float Imu::get_acc_x() {
	return data.ax;
}

float Imu::get_acc_y() {
	return data.ay;
}

float Imu::get_acc_z() {
	return data.az;
}

float Imu::get_gyro_x() {
	return data.gx;
}

float Imu::get_gyro_y() {
	return data.gx;
}

float Imu::get_gyro_z() {
	return data.gz;
}

float Imu::get_mag_x() {
	return data.mx;
}

float Imu::get_mag_y() {
	return data.my;
}


float Imu::get_mag_z() {
	return data.mz;
}
