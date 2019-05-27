#ifndef PHOTONIC_TELEMETRY_ARDUINO_HEAP_H
#define PHOTONIC_TELEMETRY_ARDUINO_HEAP_H

#include "telemetry/heap.h"

#ifdef ARDUINO
	#include <EEPROM.h>
#endif

namespace photonic {

/**
	A HeapIO implementation for use with Arduino EEPROM.
*/
class ArduinoHeapIO : public HeapIO {
public:
	/**
		@brief creates a new memory handler
	*/
	ArduinoHeapIO() {}

	/**
		@brief tears down the memory handler
	*/
	~ArduinoHeapIO() {}

	/**
		Writes a single byte to an address in memory.

		@param addr address
		@param b byte to write
		@return if writing succeeded
	*/
	bool write(int addr, byte b);

	/**
		Reads a single byte from an address in memory.

		@param addr address
		@return byte at addr
	*/
	byte read(int addr);
};

}; // end namespace photonic

#endif
