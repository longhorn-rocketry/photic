#ifndef PHOTIC_MATH_ROCKET_MATH_H
#define PHOTIC_MATH_ROCKET_MATH_H

#include <math.h>

namespace photic {

/**
 * @brief Gets if two numbers are approximately equal.
 */
bool approx(float k_a, float k_b, float k_negligence);

/**
 * The hypsometric formula, used to compute GL altitude when under 11 km.
 *
 * @param  k_p0 initial pressure, likely on launchpad (kP)
 * @param  k_p  current pressure (kP)
 * @param  k_t  current temperature (C)
 * @return      approximate GL altitude
 */
float hypso(float k_p0, float k_p, float k_t);

/**
  Computes the density of an ideal gas according to the Ideal Gas Law.

  @param k_p pressure (pascals)
  @param k_t temperature (kelvin)
  @return    density (moles / m^3)
*/
/**
 * Computes the density of a gas according to the Ideal Gas Law.
 *
 * @param  k_p pressure (pascals)
 * @param  k_t temperature (kelvin)
 * @return     density (moles/m^3)
 */
float igl_density(float k_p, float k_t);

} // end namespace photic

#endif
