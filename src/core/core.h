#ifndef PHOTONIC_CORE_H
#define PHOTONIC_CORE_H

#include "core/timekeeping.h"
#include "sensor/barometer.h"
#include "sensor/imu.h"

#include <math.h>

namespace photonic {

enum config {
	ROCKET_IGNITION_G_TRIGGER,
	ROCKET_NO_IGNITION_GRACE_PERIOD,

	ROCKET_WAIT_FOR_LIFTOFF_MA_SIZE,

	ROCKET_PRIMARY_IMU,
	ROCKET_PRIMARY_BAROMETER,
	ROCKET_TIMEKEEPER,

	ROCKET_VERTICAL_IMU_AXIS
};

enum axis {
	X,
	Y,
	Z
};

bool __photonic_has_initialized = false;
float __photonic_epoch_time = -1;
float __rocket_ignition_time = -1;

float __rocket_ignition_g_trigger = 0;
float __rocket_no_ignition_grace_period = 0;

int __rocket_wait_for_liftoff_ma_size = 10;

Imu *__rocket_primary_imu = nullptr;
Barometer *__rocket_primary_barometer = nullptr;
Timekeeper *__rocket_timekeeper = nullptr;

axis __rocket_vertical_imu_axis = Z;

/**
	@brief one-time initialization function
*/
void photonic_init();

/**
	@brief gets the number of seconds since Photonic was initialized
*/
float rocket_time() {
	return __rocket_timekeeper->time() - __photonic_epoch_time;
}

/**
	@brief gets the number of seconds since liftoff
*/
float flight_time() {
	return __rocket_ignition_time == -1 ? -1 : __rocket_timekeeper->time() -
			__rocket_ignition_time;
}

/**
	@brief configuration for global float parameters
*/
void photonic_configure(config c, float f) {
	if (!__photonic_has_initialized)
		photonic_init();

	if (c == ROCKET_IGNITION_G_TRIGGER)
		__rocket_ignition_g_trigger = f;
	else if (c == ROCKET_NO_IGNITION_GRACE_PERIOD)
		__rocket_no_ignition_grace_period = f;
}

/**
	@brief configuration for global int parameters
*/
void photonic_configure(config c, int i) {
	if (!__photonic_has_initialized)
		photonic_init();

	if (c == ROCKET_WAIT_FOR_LIFTOFF_MA_SIZE)
		__rocket_wait_for_liftoff_ma_size = i;
}

/**
	@brief configuration for global pointer parameters
*/
void photonic_configure(config c, void *ptr) {
	if (!__photonic_has_initialized)
		photonic_init();

	if (c == ROCKET_PRIMARY_IMU)
		__rocket_primary_imu = (Imu*)ptr;
	else if (c == ROCKET_PRIMARY_BAROMETER)
		__rocket_primary_barometer = (Barometer*)ptr;
	else if (c == ROCKET_TIMEKEEPER)
		__rocket_timekeeper = (Timekeeper*)ptr;
}

/**
	@brief configuration for global axis parameters
*/
void photonic_configure(config c, axis a) {
	if (!__photonic_has_initialized)
		photonic_init();

	if (c == ROCKET_VERTICAL_IMU_AXIS)
		__rocket_vertical_imu_axis = a;
}

/**
	@brief blocks program flow until the IMU reads a sustained acceleration over
	       the configured G trigger
*/
void wait_for_liftoff() {
	if (!__photonic_has_initialized || __rocket_primary_imu == nullptr ||
			__rocket_timekeeper == nullptr)
		return;

	float time_start = __rocket_timekeeper->time();
	float *accels = new float[__rocket_wait_for_liftoff_ma_size];
	float accel_avg = 0;
	int readings = 0;

	while (readings < __rocket_wait_for_liftoff_ma_size ||
			fabs(accel_avg) < __rocket_ignition_g_trigger ||
			__rocket_timekeeper->time() - time_start <= __rocket_no_ignition_grace_period) {
		// Add new accel value
		float accel = 0;
		switch (__rocket_vertical_imu_axis) {
			case X:
				accel = __rocket_primary_imu->get_acc_x();
				break;
			case Y:
				accel = __rocket_primary_imu->get_acc_y();
				break;
			case Z:
				accel = __rocket_primary_imu->get_acc_z();
				break;
		}
		accels[readings++ % __rocket_wait_for_liftoff_ma_size] = accel;

		// Find avg accel
		float accel_total = 0;
		for (int i = 0; i < __rocket_wait_for_liftoff_ma_size; i++)
			accel_total += accels[__rocket_wait_for_liftoff_ma_size];
		accel_avg = accel_total / __rocket_wait_for_liftoff_ma_size;
	}

	__rocket_ignition_time = __rocket_timekeeper->time();
}

void photonic_init() {
	__photonic_has_initialized = true;

	// If the client is running Arduino, we know what timekeeper to use
#ifdef ARDUINO
	photonic_configure(ROCKET_TIMEKEEPER, new ArduinoTimekeeper());
#endif

	__photonic_epoch_time = __rocket_timekeeper->time();
}

}; // end namespace photonic

#endif
