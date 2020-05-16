/**
 * Conversion between 32-bit and 16-bit floats. 16-bit floats have 10 mantissa
 * bits and a maximum value of 6.55x10^4, which is acceptable for most
 * high-power rocketry applications.
 *
 * Short floats are primarily intended for purposes of telemetry compression.
 * As they are just typedef'd unsigned shorts, there can be no meaningful,
 * direct arithmetic.
 *
 * Loss of precision by compressing and decompressing float32s to float16s
 * becomes more drastic with larger numbers. Loss will be negligible with
 * subnormals, on the order of 10^-1 for numbers in the hundreds, and on the
 * order of 10^1 for numbers nearing the limits.
 */

#ifndef PHOTIC_UTIL_FLOAT16_H
#define PHOTIC_UTIL_FLOAT16_H

namespace photic
{

typedef unsigned short float16;

/**
 * @brief float32 to float16 compression.
 */
float16 f32to16(float k_f32);

/**
 * @brief float16 to float32 decompression.
 */
float f16to32(float16 k_f16);

} // namespace photic

#endif
