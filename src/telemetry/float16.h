#ifndef PHOTIC_TELEMETRY_FLOAT16_H
#define PHOTIC_TELEMETRY_FLOAT16_H

namespace photic {

typedef unsigned short float16;

/**
 * Conversion utils for short floats and regular floats with 10 mantissa bits
 * and 23 mantissa bits, respectively. Maximum value of a short float is
 * 6.55x10^4, which is acceptable for most high-power rocketry applications.
 */
namespace float16_util {

	/**
	 * @brief float32 to float16 compression.
	 */
	float16 compress(float value);

	/**
	 * @brief float16 to float32 decompression.
	 */
	float decompress(float16 value);

} // end namespace float16_util

} // end namespace photic

#endif
