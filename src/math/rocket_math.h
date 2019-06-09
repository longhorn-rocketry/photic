#ifndef PHOTONIC_MATH_ROCKET_MATH_H
#define PHOTONIC_MATH_ROCKET_MATH_H

#include <math.h>

namespace photonic {

/**
	Gets if two numbers are approximately equal.

	@param a value A
	@oaram b value B
	@param negligence maximum allowed difference
	@return if fabs(a - b) <= negligence
*/
bool approx(float a, float b, float negligence);

/**
  The Hypsometric formula; used to compute GL altitude when under 11 km.

  @param p0 initial pressure, likely on launchpad (kP)
  @param p current pressure (kP)
  @param t current temperature (C)
  @return approximate current GL altitude
*/
float hypso(float p0, float p, float t);

/**
  Computes the density of an ideal gas according to the Ideal Gas Law.

  @param p pressure (pascals)
  @param t temperature (kelvin)
  @return density (moles / m^3)
*/
float igl_density(float p, float t);

}; // end namespace photonic

#endif
