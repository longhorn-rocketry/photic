#include "arduino_heap.h"

namespace photic {

bool ArduinoHeapIO::write(int k_addr, byte k_b) {
#ifdef ARDUINO
	EEPROM.write(k_addr, k_b);
	return true;
#else
	return false;
#endif
}

byte ArduinoHeapIO::read(int k_addr) {
#ifdef ARDUINO
	return EEPROM.read(k_addr);
#else
	return 0;
#endif
}

} // end namespace photic
