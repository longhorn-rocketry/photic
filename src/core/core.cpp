#include "core.h"
#include "../math/rocket_math.h"

#include <math.h>

namespace photic {

bool has_initialized = false;;

float t_epoch = -1;
float t_ignition = -1;
float t_burnout = -1;
float t_apogee = -1;

bool event_ignition;
bool event_burnout = false;
bool event_apogee = false;

float ignition_g_trigger = 0;
float ignition_g_trigger_negl = 0;
float no_ignition_grace_period = 0;

bool trigger_burnout_on_neg_accel = false;
float trigger_burnout_on_neg_accel_negl = 0;
bool trigger_burnout_on_timeout = false;
float trigger_burnout_on_timeout_val = 0;

bool trigger_apogee_on_neg_vel = false;
float trigger_apogee_on_neg_vel_negl = 0;
bool trigger_apogee_on_timeout = false;
float trigger_apogee_on_timeout_val = 0;

Imu* primary_imu = nullptr;
Barometer* primary_barometer = nullptr;
TelemetryHeap* telemetry_heap = nullptr;

history<float>* vertical_accel_history = nullptr;
history<float>* vertical_velocity_history = nullptr;
history<float>* altitude_history = nullptr;

Axis vertical_axis = Z;
Timekeeper* timekeeper = nullptr;
MicrocontrollerModel microcontroller_model = NONE;

void start() {
  if (has_initialized)
    return;

  has_initialized = true;
  t_epoch = timekeeper->time();
  t_ignition = -1;
  t_burnout = -1;
  t_apogee = -1;
  event_ignition;
  event_burnout = false;
  event_apogee = false;
}

void restart() {
  has_initialized = false;
  start();
}

float rocket_time() {
  return timekeeper->time() - t_epoch;
}

float flight_time() {
  return timekeeper->time() - t_ignition;
}

bool config(ConfigParameter k_param, double k_val) {
  if (k_param == ROCKET_IGNITION_G_TRIGGER)
    ignition_g_trigger = k_val;
  else if (k_param == ROCKET_IGNITION_G_TRIGGER_NEGL)
    ignition_g_trigger_negl = k_val;
  else if (k_param == ROCKET_NO_IGNITION_GRACE_PERIOD)
    no_ignition_grace_period = k_val;
  else if (k_param == ROCKET_TRIGGER_BURNOUT_ON_NEG_ACCEL_NEGL)
    trigger_burnout_on_neg_accel_negl = k_val;
  else if (k_param == ROCKET_TRIGGER_BURNOUT_ON_TIMEOUT_VAL)
    trigger_burnout_on_timeout_val = k_val;
  else if (k_param == ROCKET_TRIGGER_APOGEE_ON_NEG_VEL_NEGL)
    trigger_apogee_on_neg_vel_negl = k_val;
  else if (k_param == ROCKET_TRIGGER_APOGEE_ON_TIMEOUT_VAL)
    trigger_apogee_on_timeout_val = k_val;
  else
    return false;

  return true;
}

bool config(ConfigParameter k_param, bool k_b) {
  if (k_param == ROCKET_TRIGGER_BURNOUT_ON_NEG_ACCEL)
    trigger_burnout_on_neg_accel = k_b;
  else if (k_param == ROCKET_TRIGGER_BURNOUT_ON_TIMEOUT)
    trigger_burnout_on_timeout = k_b;
  else if (k_param == ROCKET_TRIGGER_APOGEE_ON_NEG_VEL)
    trigger_apogee_on_neg_vel = k_b;
  else if (k_param == ROCKET_TRIGGER_APOGEE_ON_TIMEOUT)
    trigger_apogee_on_timeout = k_b;
  else
    return false;

  return true;
}

bool config(ConfigParameter k_param, void* k_ptr) {
  if (k_param == ROCKET_PRIMARY_IMU) {
    primary_imu = (Imu*)k_ptr;
    primary_imu->initialize();
  } else if (k_param == ROCKET_PRIMARY_BAROMETER) {
    primary_barometer = (Barometer*)k_ptr;
    primary_barometer->initialize();
  } else if (k_param == ROCKET_TELEMETRY_HEAP)
    telemetry_heap = (TelemetryHeap*)k_ptr;
  else if (k_param == ROCKET_VERTICAL_ACCEL_HISTORY)
    vertical_accel_history = (history<float>*)k_ptr;
  else if (k_param == ROCKET_VERTICAL_VELOCITY_HISTORY)
    vertical_velocity_history = (history<float>*)k_ptr;
  else if (k_param == ROCKET_ALTITUDE_HISTORY)
    altitude_history = (history<float>*)k_ptr;
  else if (k_param == ROCKET_TIMEKEEPER) {
    timekeeper = (Timekeeper*)k_ptr;
    start();
  } else
    return false;

  return true;
}

bool config(ConfigParameter k_param, Axis k_axis) {
  if (k_param == ROCKET_VERTICAL_AXIS)
    vertical_axis = k_axis;
  else
    return false;

  return true;
}

bool config(ConfigParameter k_param, MicrocontrollerModel k_model) {
  if (k_param == ROCKET_MICROCONTROLLER_MODEL) {
    microcontroller_model = k_model;

    if (k_model != OTHER && k_model != NONE)
      config(ROCKET_TIMEKEEPER, new photic::ArduinoTimekeeper());
  } else
    return false;

  return true;
}

bool check_for_liftoff() {
  if (event_ignition)
    return true;

  if (rocket_time() < no_ignition_grace_period)
    return false;

  if (vertical_accel_history->at_capacity()) {
    bool detected;
    float accel = vertical_accel_history->mean();
    detected = accel + ignition_g_trigger_negl > ignition_g_trigger;

    if (detected && !event_ignition) {
      event_ignition = true;
      t_ignition = timekeeper->time();
    }
  }

  return event_ignition;
}

bool check_for_burnout() {
  if (event_burnout)
    return true;

  bool detected;

  if (trigger_burnout_on_neg_accel && vertical_accel_history->at_capacity()) {
    float accel = vertical_accel_history->mean();
    detected = accel - trigger_burnout_on_neg_accel_negl < 0;
  }

  if (!detected && trigger_burnout_on_timeout) {
    float t = flight_time();
    detected = t >= trigger_burnout_on_timeout_val;
  }

  if (detected && !event_burnout) {
    event_burnout = true;
    t_burnout = timekeeper->time();
  }

  return event_burnout;
}

bool check_for_apogee() {
  if (event_apogee)
    return true;

  bool detected;

  if (trigger_apogee_on_neg_vel && vertical_velocity_history->at_capacity()) {
    float vel = vertical_velocity_history->mean();
    detected = vel - trigger_apogee_on_neg_vel_negl < 0;
  }

  if (!detected && trigger_apogee_on_timeout) {
    float t = flight_time();
    detected = t >= trigger_apogee_on_timeout_val;
  }

  if (detected && !event_apogee) {
    event_apogee = true;
    t_apogee = timekeeper->time();
  }

  return event_apogee;
}

void wait_for_liftoff() {
  history<float> hist(10);

  do {
    primary_imu->update();
    hist.add(primary_imu->get_acc_z());
  } while (!hist.at_capacity() || hist.mean() < ignition_g_trigger);
}

} // end namespace photic
