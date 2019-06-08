#include "timekeeping.h"

#ifdef ARDUINO
	#include <Arduino.h>
#endif

using namespace photonic;

float ArduinoTimekeeper::time() {
#ifdef ARDUINO
	return millis() / 1000.0;
#else
	return -1;
#endif
}
