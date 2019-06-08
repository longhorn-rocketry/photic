#ifndef PHOTONIC_CORE_H
#define PHOTONIC_CORE_H

#include "timekeeping.h"
#include "../sensor/barometer.h"
#include "../sensor/imu.h"
#include "../telemetry/heap.h"
#include "../telemetry/history.h"

namespace photonic {

enum config {
	ROCKET_IGNITION_G_TRIGGER,
	ROCKET_NO_IGNITION_GRACE_PERIOD,
	ROCKET_BURNOUT_DETECTION_NEGLIGENCE,
	ROCKET_APOGEE_DETECTION_NEGLIGENCE,
	ROCKET_BURNOUT_ACCELERATION,
	ROCKET_APOGEE_VELOCITY,
	ROCKET_AUTOMATIC_BURNOUT,

	ROCKET_WAIT_FOR_LIFTOFF_MA_SIZE,

	ROCKET_PRIMARY_IMU,
	ROCKET_PRIMARY_BAROMETER,
	ROCKET_TIMEKEEPER,
	ROCKET_VERTICAL_ACCEL_HISTORY,
	ROCKET_VERTICAL_VELOCITY_HISTORY,
	ROCKET_TELEMETRY_HEAP,

	ROCKET_VERTICAL_IMU_AXIS,

	ROCKET_MICROCONTROLLER,

	ROCKET_BURNOUT_DETECTION_METHOD,

	ROCKET_APOGEE_DETECTION_METHOD
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

enum burnout_detection_method {
	APPROX_BURNOUT_ACCEL,
	NEGATIVE_AVG_ACCEL
};

enum apogee_detection_method {
	APPROX_ZERO_VELOCITY,
	APPROX_FREEFALL_ACCEL
};

extern bool __photonic_has_initialized;
extern float __photonic_epoch_time;
extern float __rocket_ignition_time;

extern bool __flight_event_burnout;
extern bool __flight_event_apogee;

extern float __rocket_ignition_g_trigger;
extern float __rocket_no_ignition_grace_period;
extern float __rocket_burnout_detection_negligence;
extern float __rocket_apogee_detection_negligence;
extern float __rocket_burnout_acceleration;
extern float __rocket_apogee_velocity;
extern float __rocket_automatic_burnout;

extern int __rocket_wait_for_liftoff_ma_size;

extern Imu *__rocket_primary_imu;
extern Barometer *__rocket_primary_barometer;
extern Timekeeper *__rocket_timekeeper;
extern history<float> *__rocket_vertical_accel_history;
extern history<float> *__rocket_vertical_velocity_history;
extern TelemetryHeap *__rocket_telemetry_heap;

extern axis __rocket_vertical_imu_axis;

extern microcontroller_model __rocket_microcontroller;

extern burnout_detection_method __rocket_burnout_detection_method;

extern apogee_detection_method __rocket_apogee_detection_method;

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
	@brief configuration for apogee detection method
*/
void photonic_configure(config c, apogee_detection_method a);

/**
	@brief configuration for burnout detection method
*/
void photonic_configure(config c, burnout_detection_method b);

/**
	@brief blocks program flow until the IMU reads a sustained acceleration over
	       the configured G trigger
*/
void wait_for_liftoff();

/**
	@brief gets whether or not engine burnout has occurred
*/
bool check_for_burnout();

/**
	@brief gets whether or not apogee has occurred
*/
bool check_for_apogee();

}; // end namespace photonic

#endif
