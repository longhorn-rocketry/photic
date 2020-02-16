/**
 * Various mathematical utilities and formulas relevant to high-power rocketry.
 */

#ifndef PHOTIC_MATH_ROCKETMATH_H
#define PHOTIC_MATH_ROCKETMATH_H

#include <math.h>

namespace photic
{

/**
 * Acceleration due to gravity at Earth sea level (m/sec/sec).
 */
const float EARTH_SLGRAV_MPSSQ = 9.80665;
/**
 * Speed of sound at Earth sea level (m/sec).
 */
const float EARTH_SLMACH_MPS = 343;

/**
 * Gets if two numbers are approximately equal.
 *
 * @param   k_a          LHS.
 * @param   k_b          RHS.
 * @param   k_negligence Maximum absolute difference between k_a and k_b to
 *                       declare them approximately equal.
 *
 * @ret     True if k_a is approximately k_b, false otherwise.
 */
bool fapprox(float k_a, float k_b, float k_negligence=1e-6);

/**
 * The hypsometric formula, used to compute GL altitude when under 11 km.
 *
 * @param   k_p0 Initial pressure, likely on launchpad (kP).
 * @param   k_p  Current pressure (kP).
 * @param   k_t  Current temperature (C).
 *
 * @ret     Approximate GL altitude (m).
 */
float hypso(float k_p0, float k_p, float k_t);

/**
 * Computes the density of a gas according to the Ideal Gas Law.
 *
 * @param   k_p Pressure (P).
 * @param   k_t Temperature (K).
 *
 * @ret     Density (moles/m^3).
 */
float igl_density(float k_p, float k_t);

/**
 * Linear interpolation between two values.
 *
 * @param   k_lower Lower value.
 * @param   k_upper Upper value.
 * @param   k_p     Interpolation percentage, probably between 0 and 1.
 *
 * @ret     Interpolated value of the same type as the parameters.
 */
template <typename T>
T lerp(T k_lower, T k_upper, double k_p)
{
    return k_lower + (k_upper - k_lower) * k_p;
}

} // namespace photic

#endif
