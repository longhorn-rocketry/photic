#include "math/rocketmath.h"

namespace photic
{

bool fapprox(float k_a, float k_b, float k_negligence)
{
	return fabs(k_a - k_b) <= k_negligence;
}

float hypso(float k_p0, float k_p, float k_t)
{
 	return (pow((k_p0 / k_p), 1 / 5.257) - 1) * (k_t + 273.15) / 0.0065;
}

float igl_density(float k_p, float k_t)
{
	return k_p / (8.3144568 * k_t);
}

} // end namespace photic
