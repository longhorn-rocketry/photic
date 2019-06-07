#ifndef PHOTONIC_MATH_KALMAN_FILTER_H
#define PHOTONIC_MATH_KALMAN_FILTER_H

#include "math/matrix.h"

/**
  Kalman filter with an [altitude, acceleration] observation model as is common
  in rocketry.
*/
class KalmanFilter {
protected:
  matrix A, Q, H, P, R, K;
  matrix estimate;

public:
  /**
    @brief creates an uninitialized filter; before making filter calls, you must
           call set_delta_t, set_sensor_variance, set_initial_estimate, and
           compute_kg, in that order
  */
  KalmanFilter();

  /**
    Sets the time step used in state transitions.

    @param dt time step
  */
  void set_delta_t(float dt);

  /**
    Sets the variance in altitude and acceleration readings.

    @param altimeter_variance variance in altimeter readings
    @param accelerometer_variance variance in accelerometer readings
  */
  void set_sensor_variance(float altimeter_variance,
      float accelerometer_variance);

  /**
    Sets the initial state estimate. More often than not, this is just
    [0, 0, 0].

    @param s initial altitude
    @param v initial velocity
    @param a initial acceleration
  */
  void set_initial_estimate(float s, float v, float a);

  /**
    Recursive computation for the Kalman gain matrix. Because sensor variance
    does not change throughout flight and because the KG computation is
    expensive, this method is designed to be called only once.

    Note that calling this method resets the process covariance to the identity
    matrix and recomputes it from scratch. If you're only computing the KG once,
    don't worry about this.

    @param iterations depth of recursion; we recommend ~1000 iterations
  */
  void compute_kg(int iterations);

  /**
    Performs a single refinement on the current KG based on the current process
    covariance. This method should only be used if the variance of the vehicle's
    sensors is not constant (this is very uncommon). If the method is used,
    it should be called once per filter iteration.
  */
  void compute_kg();

  /**
    @brief sets the Kalman gain; intended for use by low-compute power
           microcontrollers that would rather load a KG computed offboard
  */
  void set_kg(matrix kg);

  /**
    Runs a single filter iteration and returns the new state estimate.

    @param s altitude reading
    @param a acceleration reading
    @return filtered state
  */
  matrix filter(float s, float a);
};

#endif
