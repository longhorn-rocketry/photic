#include "arduino_heap.h"

using namespace photonic;

bool ArduinoHeapIO::write(int addr, byte b) {
#ifdef ARDUINO
	EEPROM.write(addr, b);
	return true;
#else
	return false
#endif
}

byte ArduinoHeapIO::read(int addr) {
#ifdef ARDUINO
	return EEPROM.read(addr);
#else
	return 0;
#endif
}
