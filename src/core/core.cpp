#include "core/core.h"
#include "math/rocket_math.h"

#include <math.h>

namespace photonic {

bool __photonic_has_initialized = false;
float __photonic_epoch_time = -1;
float __rocket_ignition_time = -1;

bool __flight_event_burnout = false;

float __rocket_ignition_g_trigger = 0;
float __rocket_no_ignition_grace_period = 0;
float __rocket_burnout_detection_negligence = 0.25;

int __rocket_wait_for_liftoff_ma_size = 10;

Imu *__rocket_primary_imu = nullptr;
Barometer *__rocket_primary_barometer = nullptr;
Timekeeper *__rocket_timekeeper = nullptr;
history<float> *__rocket_vertical_accel_history = nullptr;

axis __rocket_vertical_imu_axis = Z;

microcontroller_model __rocket_microcontroller = NONE;

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

bool check_for_burnout() {
	if (__flight_event_burnout)
		return true;
	else if (__rocket_vertical_accel_history == nullptr)
		return false;

	float accel_avg = __rocket_vertical_accel_history->mean();
	if (approx(accel_avg, -1, __rocket_burnout_detection_negligence))
		__flight_event_burnout = true;

	return __flight_event_burnout;
}

}; // end namespace photonic
