#ifndef PHOTONIC_TELEMETRY_FLOAT16_H
#define PHOTONIC_TELEMETRY_FLOAT16_H

#include <cstdint>

namespace photonic {

typedef unsigned short float16;

/**
	Converts between short floats and regular floats with 10 mantissa bits and 23
	mantissa bits, respectively. Maximum value of a short float is 6.55x10^4,
	which is acceptable for most high-power rocket telemetry.

	This header and its accompanying .cpp file contain a very elegant
	implementation of the conversion by Stack Overflow user Phernost:
	https://stackoverflow.com/questions/1659440/32-bit-to-16-bit-floating-point-conversion
*/
class Float16Compressor {
protected:
	union Bits {
		float f;
		int32_t si;
		uint32_t ui;
	};

	static int const shift = 13;
	static int const shiftSign = 16;

	static int32_t const infN = 0x7F800000; // flt32 infinity
	static int32_t const maxN = 0x477FE000; // max flt16 normal as a flt32
	static int32_t const minN = 0x38800000; // min flt16 normal as a flt32
	static int32_t const signN = 0x80000000; // flt32 sign bit

	static int32_t const infC = infN >> shift;
	static int32_t const nanN = (infC + 1) << shift; // minimum flt16 nan as a flt32
	static int32_t const maxC = maxN >> shift;
	static int32_t const minC = minN >> shift;
	static int32_t const signC = signN >> shiftSign; // flt16 sign bit

	static int32_t const mulN = 0x52000000; // (1 << 23) / minN
	static int32_t const mulC = 0x33800000; // minN / (1 << (23 - shift))

	static int32_t const subC = 0x003FF; // max flt32 subnormal down shifted
	static int32_t const norC = 0x00400; // min flt32 normal down shifted

	static int32_t const maxD = infC - maxC - 1;
	static int32_t const minD = minC - subC - 1;

public:
	/**
		@brief regular float to short float compression
	*/
	static float16 compress(float value);

	/**
		@brief short float to regular float decompression
	*/
	static float decompress(uint16_t value);
};

}; // end namespace photonic

#endif
