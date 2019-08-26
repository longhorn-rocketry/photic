#ifndef PHOTIC_TELEMETRY_ARDUINO_HEAP_H
#define PHOTIC_TELEMETRY_ARDUINO_HEAP_H

#include "heap.h"

#ifdef ARDUINO
	#include <EEPROM.h>
#endif

namespace photic {

/**
 * A HeapIO implementation for Arduino EEPROM.
 */
class ArduinoHeapIO : public HeapIO {
public:
	ArduinoHeapIO() {}

	~ArduinoHeapIO() {}

	bool write(int k_addr, byte k_b);

	byte read(int k_addr);
};

} // end namespace photic

#endif
