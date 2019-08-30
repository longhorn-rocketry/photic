#ifndef PHOTIC_CORE_H
#define PHOTIC_CORE_H

#include <math.h>

#include "timekeeping.h"
#include "../sensor/barometer.h"
#include "../sensor/imu.h"
#include "../telemetry/heap.h"
#include "../telemetry/history.h"

enum PhoticConfigParameter {
  // Ignition detection
  ROCKET_IGNITION_G_TRIGGER,
  ROCKET_IGNITION_G_TRIGGER_NEGL,
  ROCKET_IGNITION_G_TRIGGER_MASZ,
  ROCKET_NO_IGNITION_GRACE_PERIOD,

  // Burnout detection
  ROCKET_TRIGGER_BURNOUT_ON_NEG_ACCEL,
  ROCKET_TRIGGER_BURNOUT_ON_NEG_ACCEL_NEGL,
  ROCKET_TRIGGER_BURNOUT_ON_TIMEOUT,
  ROCKET_TRIGGER_BURNOUT_ON_TIMEOUT_VAL,

  // Apogee detection
  ROCKET_TRIGGER_APOGEE_ON_NEG_VEL,
  ROCKET_TRIGGER_APOGEE_ON_NEG_VEL_NEGL,
  ROCKET_TRIGGER_APOGEE_ON_TIMEOUT,
  ROCKET_TRIGGER_APOGEE_ON_TIMEOUT_VAL,

  // Hardware
  ROCKET_PRIMARY_IMU,
  ROCKET_PRIMARY_BAROMETER,
  ROCKET_TELEMETRY_HEAP,

  // Sampling
  ROCKET_VERTICAL_ACCEL_HISTORY,
  ROCKET_VERTICAL_VELOCITY_HISTORY,
  ROCKET_ALTITUDE_HISTORY,

  // Misc
  ROCKET_VERTICAL_AXIS,
  ROCKET_TIMEKEEPER,
  ROCKET_MICROCONTROLLER_MODEL
};

namespace photic {

enum Axis {
  X,
  Y,
  Z
};

enum MicrocontrollerModel {
  NONE,
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
  OTHER
};

extern bool has_initialized;

extern float t_epoch;
extern float t_ignition;
extern float t_burnout;
extern float t_apogee;

extern bool event_ignition;
extern bool event_burnout;
extern bool event_apogee;

extern float ignition_g_trigger;
extern float ignition_g_trigger_negl;
extern float no_ignition_grace_period;

extern bool trigger_burnout_on_neg_accel;
extern float trigger_burnout_on_neg_accel_negl;
extern bool trigger_burnout_on_timeout;
extern float trigger_burnout_on_timeout_val;

extern bool trigger_apogee_on_neg_vel;
extern float trigger_apogee_on_neg_vel_negl;
extern bool trigger_apogee_on_timeout;
extern float trigger_apogee_on_timeout_val;

extern Imu* primary_imu;
extern Barometer* primary_barometer;
extern TelemetryHeap* telemetry_heap;

extern history<float>* vertical_accel_history;
extern history<float>* vertical_velocity_history;
extern history<float>* altitude_history;

extern Axis vertical_axis;
extern Timekeeper* timekeeper;
extern MicrocontrollerModel microcontroller_model;

/**
 * Sets the epoch time and does other one-time initialization stuff. Called
 * automatically when ROCKET_TIMEKEEPER is configured.
 */
void start();

/**
 * Resets the flight (events, times, etc.).
 */
void restart();

/**
 * @brief Gets the time since photic epoch (negative if not yet initialized).
 */
float rocket_time();

/**
 * @brief Gets the time since ignition detection (negative if not yet detected).
 */
float flight_time();

/**
 * @brief Configures integral photic parameters.
 */
bool config(PhoticConfigParameter k_param, double k_val);

/**
 * @brief Configures boolean photic parameters.
 */
bool config(PhoticConfigParameter k_param, bool k_b);

/**
 * @brief Configures pointer photic parameters.
 */
bool config(PhoticConfigParameter k_param, void* k_ptr);

/**
 * @brief Configures axis photic parameters.
 */
bool config(PhoticConfigParameter k_param, Axis k_axis);

/**
 * @brief Configures microcontroller photic parameters.
 */
bool config(PhoticConfigParameter k_param, MicrocontrollerModel k_model);

/**
 * Gets whether or not liftoff has been detected. If it has not been, an attempt
 * at detecting it is made.
 *
 * @return liftoff detected
 */
bool check_for_liftoff();

/**
 * Gets whether or not burnout has been detected. If it has not been, an attempt
 * at detecting it is made.
 *
 * @return burnout detected
 */
bool check_for_burnout();

/**
 * Gets whether or not apogee has been detected. If it has not been, an attempt
 * at detecting it is made.
 *
 * @return apogee detected
 */
bool check_for_apogee();

/**
 * @brief Simple, one-line liftoff detection with ROCKET_PRIMARY_IMU.
 */
void wait_for_liftoff();

} // end namespace photic

#endif
