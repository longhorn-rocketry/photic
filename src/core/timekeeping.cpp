#include "timekeeping.h"

#ifdef ARDUINO
	#include <Arduino.h>
#endif

namespace photic {

float ArduinoTimekeeper::time() {
#ifdef ARDUINO
	return millis() / 1000.0;
#else
	return -1;
#endif
}

} // end namespace photic
