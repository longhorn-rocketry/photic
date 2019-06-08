#include "core.h"
#include "../math/rocket_math.h"

#include <math.h>

namespace photonic {

bool __photonic_has_initialized = false;
float __photonic_epoch_time = -1;
float __rocket_ignition_time = -1;

bool __flight_event_burnout = false;
bool __flight_event_apogee = false;

float __rocket_ignition_g_trigger = 0;
float __rocket_no_ignition_grace_period = 0;
float __rocket_burnout_detection_negligence = 0.25;
float __rocket_apogee_detection_negligence = 0.25;
float __rocket_burnout_acceleration = -1;
float __rocket_apogee_velocity = 0;
float __rocket_automatic_burnout = -1;

int __rocket_wait_for_liftoff_ma_size = 10;

Imu *__rocket_primary_imu = nullptr;
Barometer *__rocket_primary_barometer = nullptr;
Timekeeper *__rocket_timekeeper = nullptr;
history<float> *__rocket_vertical_accel_history = nullptr;
history<float> *__rocket_vertical_velocity_history = nullptr;
TelemetryHeap *__rocket_telemetry_heap = nullptr;

axis __rocket_vertical_imu_axis = Z;

microcontroller_model __rocket_microcontroller = NONE;

apogee_detection_method __rocket_apogee_detection_method =
		APPROX_FREEFALL_ACCEL;

burnout_detection_method __rocket_burnout_detection_method =
		NEGATIVE_AVG_ACCEL;

void photonic_init() {
	__photonic_has_initialized = true;

	// If the client is running Arduino, we know what timekeeper to use
#ifdef ARDUINO
	photonic_configure(ROCKET_TIMEKEEPER, new ArduinoTimekeeper());
#endif

	__photonic_epoch_time = __rocket_timekeeper->time();
}

float rocket_time() {
	return __rocket_timekeeper->time() - __photonic_epoch_time;
}

float flight_time() {
	return __rocket_ignition_time == -1 ? -1 : __rocket_timekeeper->time() -
			__rocket_ignition_time;
}

void photonic_configure(config c, float f) {
	if (!__photonic_has_initialized)
		photonic_init();

	if (c == ROCKET_IGNITION_G_TRIGGER)
		__rocket_ignition_g_trigger = f;
	else if (c == ROCKET_NO_IGNITION_GRACE_PERIOD)
		__rocket_no_ignition_grace_period = f;
	else if (c == ROCKET_BURNOUT_DETECTION_NEGLIGENCE)
		__rocket_burnout_detection_negligence = f;
	else if (c == ROCKET_APOGEE_DETECTION_NEGLIGENCE)
		__rocket_apogee_detection_negligence = f;
	else if (c == ROCKET_BURNOUT_ACCELERATION)
		__rocket_burnout_acceleration = f;
	else if (c == ROCKET_APOGEE_VELOCITY)
		__rocket_apogee_velocity = f;
	else if (c == ROCKET_AUTOMATIC_BURNOUT)
		__rocket_automatic_burnout = f;
}

void photonic_configure(config c, int i) {
	if (!__photonic_has_initialized)
		photonic_init();

	if (c == ROCKET_WAIT_FOR_LIFTOFF_MA_SIZE)
		__rocket_wait_for_liftoff_ma_size = i;
}

void photonic_configure(config c, void *ptr) {
	if (!__photonic_has_initialized)
		photonic_init();

	if (c == ROCKET_PRIMARY_IMU)
		__rocket_primary_imu = (Imu*)ptr;
	else if (c == ROCKET_PRIMARY_BAROMETER)
		__rocket_primary_barometer = (Barometer*)ptr;
	else if (c == ROCKET_TIMEKEEPER)
		__rocket_timekeeper = (Timekeeper*)ptr;
	else if (c == ROCKET_VERTICAL_ACCEL_HISTORY)
		__rocket_vertical_accel_history = (history<float>*)ptr;
	else if (c == ROCKET_VERTICAL_VELOCITY_HISTORY)
		__rocket_vertical_velocity_history = (history<float>*)ptr;
	else if (c == ROCKET_TELEMETRY_HEAP) {
		__rocket_telemetry_heap = (TelemetryHeap*)ptr;
		if (__rocket_telemetry_heap != nullptr)
			__rocket_telemetry_heap->auto_configure();
	}
}

void photonic_configure(config c, axis a) {
	if (!__photonic_has_initialized)
		photonic_init();

	if (c == ROCKET_VERTICAL_IMU_AXIS)
		__rocket_vertical_imu_axis = a;
}

void photonic_configure(config c, microcontroller_model m) {
	if (!__photonic_has_initialized)
		photonic_init();

	if (c == ROCKET_MICROCONTROLLER)
		__rocket_microcontroller = m;
}

void photonic_configure(config c, apogee_detection_method a) {
	if (!__photonic_has_initialized)
		photonic_init();

	if (c == ROCKET_APOGEE_DETECTION_METHOD)
		__rocket_apogee_detection_method = a;
}

void photonic_configure(config c, burnout_detection_method a) {
	if (!__photonic_has_initialized)
		photonic_init();

	if (c == ROCKET_BURNOUT_DETECTION_METHOD)
		__rocket_burnout_detection_method = a;
}

void wait_for_liftoff() {
	if (!__photonic_has_initialized ||
		  __rocket_primary_imu == nullptr ||
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
		__rocket_primary_imu->update();
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
		accels[readings % __rocket_wait_for_liftoff_ma_size] = accel;
		readings++;

		// Find avg accel
		float accel_total = 0;
		for (int i = 0; i < __rocket_wait_for_liftoff_ma_size; i++)
			accel_total += accels[__rocket_wait_for_liftoff_ma_size];
		accel_avg = accel_total / __rocket_wait_for_liftoff_ma_size;
	}

	__rocket_ignition_time = __rocket_timekeeper->time();
}

bool check_for_burnout() {
	if (__flight_event_burnout)
		return true;
	else if (__rocket_vertical_accel_history == nullptr)
		return false;

	if (__rocket_burnout_detection_method == NEGATIVE_AVG_ACCEL) {
		if (__rocket_vertical_accel_history->at_capacity() &&
		 		__rocket_vertical_accel_history->mean() < 0)
			__flight_event_burnout = true;
	} else if (__rocket_burnout_detection_method == APPROX_BURNOUT_ACCEL) {
		if (__rocket_vertical_accel_history->at_capacity() &&
				approx(__rocket_vertical_accel_history->mean(),
						   __rocket_burnout_acceleration,
						   __rocket_burnout_detection_negligence))
			__flight_event_burnout = true;
	}

	if (__rocket_automatic_burnout != -1 &&
		  flight_time() > __rocket_automatic_burnout)
		__flight_event_burnout = true;

	return __flight_event_burnout;
}

bool check_for_apogee() {
	if (__flight_event_apogee)
		return true;

	if (__rocket_apogee_detection_method == APPROX_ZERO_VELOCITY) {
		float velocity_avg = __rocket_vertical_velocity_history->mean();
		if (__rocket_vertical_velocity_history->at_capacity() &&
				approx(velocity_avg,
					     __rocket_apogee_velocity,
						   __rocket_apogee_detection_negligence))
			__flight_event_apogee = true;
	} else if (__rocket_apogee_detection_method == APPROX_FREEFALL_ACCEL) {
		float accel_avg = __rocket_vertical_accel_history->mean();
		if (__rocket_vertical_accel_history->at_capacity() &&
				approx(accel_avg,
					     -1,
							 __rocket_apogee_detection_negligence))
			__flight_event_apogee = true;
	}

	return __flight_event_apogee;
}

}; // end namespace photonic
