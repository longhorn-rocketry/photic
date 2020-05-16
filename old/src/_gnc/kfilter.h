/**
 * 1 DoF Kalman filter designed for high-power rocketry applications. Barometric
 * altitude estimate and vertical acceleration reading go in, filtered state
 * (altitude, velocity, acceleration) comes out.
 *
 * Based on "Digital Detection of Rocket Apogee" by Dougal et al.
 * http://cnx.org/content/col11599/1.1/
 *
 * This filter has been used extensively in simulated 1 DoF high-power flights.
 * Some notable observations about its behavior and performance:
 *
 *   - When using an unchanging covariance matrix, the filter responds poorly to
 *     dramatic changes in the update vector, e.g. between liftoff and motor
 *     burnout. The greatest accuracy was seen during the coast phase.
 *   - The rate at which error is driven to zero is greatly dependent on the
 *     depth of the covarince matrix calculation. This depth may need to be
 *     tuned somewhat.
 *   - Filter accuracy does not approach perfect as the depth of the covariance
 *     matrix calculation approaches infinity. There is a balance to be struck.
 */

#ifndef PHOTIC_GNC_KFILTER_H
#define PHOTIC_GNC_KFILTER_H

#include <stdint.h>

#include "math/matrix.h"

namespace photic
{

class KalmanFilter
{
protected:
    /**
     * State transition matrix.
     */
    matrix m_A;
    /**
     * Process noise covariance. Unused.
     */
    matrix m_Q;
    /**
     * Mapping of state to observtions.
     */
    matrix m_H;
    /**
     * Error covariance.
     */
    matrix m_P;
    /**
     * Measurement noise covariance.
     */
    matrix m_R;
    /**
     * Kalman gain.
     */
    matrix m_K;
    /**
     * Last computed state estimate.
     */
    matrix m_estimate;

public:
  /**
   * Creates an uninitialized filter. Before filtering begins, the following
   * methods must be called:
   *
   *   - set_delta_t()
   *   - set_sensor_variance()
   *   - set_initial_estimate()
   *   - compute_kg() (this must be last)
   */
  KalmanFilter();

  /**
   * Sets the timestep used in state transitions.
   *
   * @param   k_dt Timestep size.
   */
  void set_delta_t(float k_dt);

  /**
   * Sets the variance in state observations.
   *
   * @param   k_altimeter_variance     Variance in altimeter readings.
   * @param   k_accelerometer_variance Variance in accelerometer readings.
   */
  void set_sensor_variance(float k_altimeter_variance,
                           float k_accelerometer_variance);

  /**
   * Sets the initial state estimate. More often that not, this is just
   * (0, 0, 0).
   *
   * @param   k_s Initial altitude.
   * @param   k_v Initial velocity.
   * @param   k_a Initial acceleration.
   */
  void set_initial_estimate(float k_s, float k_v, float k_a);

  /**
   * Computes the Kalman gain matrix. Because sensor variance is unlikely to
   * change throughout flight (and because the computing KG is expensive), this
   * method is ideally called once per flight.
   *
   * @param   k_iterations Number of iterations in computation. The ideal number
   *                       of iterations may require some trial and error.
   */
  void compute_kg(uint32_t k_iterations);

  /**
   * Performs a single refinement on the current KG based on the current process
   * covariance. This method should only be used if the variance in the
   * vehicle's sensors is not constant. If this method is used, it should be
   * called once per state update, along with set_sensor_variance() to set the
   * new variances.
   */
  void compute_kg();

  /**
   * Overrides the Kalman gain. Use this if you're precomputing KG.
   *
   * @param   k_kg Kalman gain matrix.
   */
  void set_kg(matrix k_kg);

  /**
   * Runs a single filter iteration and returns the new state estimate.
   *
   * @param   k_s Observed altitude.
   * @param   k_a Observed acceleration.
   *
   * @ret     Estimated state.
   */
  matrix filter(float k_s, float k_a);
};

} // namespace photic

#endif
