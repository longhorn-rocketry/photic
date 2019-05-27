#ifndef PHOTONIC_CORE_H
#define PHOTONIC_CORE_H

#include "core/timekeeping.h"
#include "sensor/barometer.h"
#include "sensor/imu.h"

namespace photonic {

enum config {
	ROCKET_IGNITION_G_TRIGGER,
	ROCKET_NO_IGNITION_GRACE_PERIOD,

	ROCKET_WAIT_FOR_LIFTOFF_MA_SIZE,

	ROCKET_PRIMARY_IMU,
	ROCKET_PRIMARY_BAROMETER,
	ROCKET_TIMEKEEPER,

	ROCKET_VERTICAL_IMU_AXIS,

	ROCKET_MICROCONTROLLER
};

enum axis {
	X,
	Y,
	Z
};

enum microcontroller_model {
	TEENSY_36,
	TEENSY_35,
	TEENSY_32,
	TEENSY_31,
	TEENSY_30,
	TEENSY_LC,
	TEENSYPP_20,
	TEENSY_20,
	TEENSYPP_10,
	TEENSY_10,
	NONE
};

extern bool __photonic_has_initialized;
extern float __photonic_epoch_time;
extern float __rocket_ignition_time;

extern float __rocket_ignition_g_trigger;
extern float __rocket_no_ignition_grace_period;

extern int __rocket_wait_for_liftoff_ma_size;

extern Imu *__rocket_primary_imu;
extern Barometer *__rocket_primary_barometer;
extern Timekeeper *__rocket_timekeeper;

extern axis __rocket_vertical_imu_axis;

extern microcontroller_model __rocket_microcontroller;

/**
	@brief one-time initialization function
*/
void photonic_init();

/**
	@brief gets the number of seconds since Photonic was initialized
*/
float rocket_time();

/**
	@brief gets the number of seconds since liftoff
*/
float flight_time();

/**
	@brief configuration for global float parameters
*/
void photonic_configure(config c, float f);

/**
	@brief configuration for global int parameters
*/
void photonic_configure(config c, int i);

/**
	@brief configuration for global pointer parameters
*/
void photonic_configure(config c, void *ptr);

/**
	@brief configuration for global axis parameters
*/
void photonic_configure(config c, axis a);

/**
	@brief configuration for global microcontroller parameters
*/
void photonic_configure(config c, microcontroller_model m);

/**
	@brief blocks program flow until the IMU reads a sustained acceleration over
	       the configured G trigger
*/
void wait_for_liftoff();

}; // end namespace photonic

#endif
