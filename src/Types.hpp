/**
 * Miscellaneous typedefs used across Photic.
 */

#ifndef PHOTIC_TYPES_HPP
#define PHOTIC_TYPES_HPP

namespace Photic
{

/**
 * cstdint numeric types.
 */
typedef char               int8_t;
typedef unsigned char      uint8_t;
typedef short              int16_t;
typedef unsigned short     uint16_t;
typedef int                int32_t;
typedef unsigned int       uint32_t;
typedef long long          int64_t;
typedef unsigned long long uint64_t;

/**
 * Denotes a real number, usually an engineering unit. Single-precision is
 * plenty for high power rocketry, where process noise is expected to have a
 * much larger order of magnitude than machine epsilon. This also cuts down on
 * compute time for microprocessors with weaker FPUs.
 */
typedef float Real_t;

/**
 * Represents the dimension of an indexed structure, e.g. list, matrix. A single
 * byte is sufficient for this, since high power software typically doesn't use
 * large data structures (more than 255 elements, in this case).
 */
typedef uint8_t Dim_t;

/**
 * Represents a time value. We default this to a real number since most rate
 * sensors report values in units per second, so it is most most convenient to
 * represent time as a floating point number of seconds.
 *
 * For users who prefer a different format, e.g. an integer number of milli or
 * nanoseconds, this type may be safely changed to support this. Photic
 * utilities which use time (e.g. Metronome, KalmanFilter) are agnostic of
 * whether this type is floating point or integral. (Note that an integral
 * Time_t will fail the Metronome unit tests.)
 */
typedef Real_t Time_t;

} // namespace Photic

#endif