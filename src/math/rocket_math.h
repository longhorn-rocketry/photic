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

}; // end namespace photonic

#endif
