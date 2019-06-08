#include "rocket_math.h"

namespace photonic {

bool approx(float a, float b, float negligence) {
	return fabs(a - b) <= negligence;
}

}; // end namespace photonic
