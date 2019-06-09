#include "rocket_math.h"

namespace photonic {

bool approx(float a, float b, float negligence) {
	return fabs(a - b) <= negligence;
}

float hypso(float p0, float p, float t) {
  return (pow((p0 / p), 1 / 5.257) - 1) * (t + 273.15) / 0.0065;
}

float igl_density(float p, float t) {
	return p / (8.3144568 * t);
}

}; // end namespace photonic
