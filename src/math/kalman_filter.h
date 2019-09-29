#ifndef PHOTIC_MATH_KALMAN_FILTER_H
#define PHOTIC_MATH_KALMAN_FILTER_H

#include "matrix.h"

namespace photic {

/**
 * 1D Kalman filter with an [altitude, acceleration] observation model.
 */
class KalmanFilter {
protected:
  matrix m_A, m_Q, m_H, m_P, m_R, m_K;
  matrix m_estimate;

public:
  /**
   * @brief Creates an uninitialized filter. Before making filter calls, you
   *        must call set_delta_t, set_sensor_covariance, set_initial_estimate,
   *        and then compute_kg (which must be last).
   */
  KalmanFilter();

  /**
   * @brief Sets the timestep used in state transitions.
   */
  void set_delta_t(float k_dt);

  /**
   * Sets the variance in altitude and acceleration readings.
   *
   * @param   k_altimeter_variance     altimeter variance
   * @param   k_accelerometer_variance accelerometer variance
   */
  void set_sensor_variance(float k_altimeter_variance,
                           float k_accelerometer_variance);

  /**
   * Sets the initial state estimate. More often that not, this is just
   * [0, 0, 0].
   *
   * @param   k_s initial altitude
   * @param   k_v initial velocity
   * @param   k_a initial acceleration
   */
  void set_initial_estimate(float k_s, float k_v, float k_a);

  /**
   * Recursive computation for the Kalman gain matrix. Because sensor variance
   * is unlikely to change throughout flight and because the computing KG is
   * expensive, this method is designed to be called once per flight.
   *
   * Note that calling this method resets the process covariance to the identity
   * matrix and recomputes it from scratch. If you're only computing the KG once,
   * don't worry about this.
   *
   * @param k_iterations computation depth (we recommend ~1000)
   */
  void compute_kg(unsigned int k_iterations);


  /**
   * Performs a single refinement on the current KG based on the current process
   * covariance. This method should only be used if the variance in the vehicle's
   * sensors is not constant. If the method is used, it should be called once per
   * system iteration.
   */
  void compute_kg();

  /**
   * @brief Overrides the Kalman gain. Use this if you're precomputing your KG.
   */
  void set_kg(matrix k_kg);

  /**
   * Runs a single filter iteration and returns the new state estimate.
   *
   * @param k_s   observed altitude
   * @param k_a   observed acceleration
   *
   * @ret         estimated state
   */
  matrix filter(float k_s, float k_a);
};

} // end namespace photic

#endif
