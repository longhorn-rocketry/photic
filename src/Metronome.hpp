/**
 * An object for running code at regular intervals. Metronomes "tick" at some
 * specified frequency and are polled with a timestamp.
 *
 *                              ---- USAGE ----
 *
 * Metronome usage is best illustrated with a code snippet.
 *
 *   Photic::Metronome met (5); // Metronome ticks every 5 time units.
 *   while (true)
 *   {
 *       Photic::Time_t tCurrent = <CURRENT TIME>;
 *       if (met.poll (tCurrent))
 *       {
 *           // Code to run every time the Metronome ticks, in this case every
 *           // 5 time units.
 *
 *           // There will always be a bit of time jitter in the program, i.e.
 *           // this code will not run precisely every 5 time units. The
 *           // following function gives a more exact elapsed time (will be
 *           // Metronome::TIME_NONE if the Metronome hasn't ticked twice yet).
 *           Time_t tElapsed = met.getDt ();
 *       }
 *   }
 */

#ifndef PHOTIC_METRONOME_HPP
#define PHOTIC_METRONOME_HPP

#include "Types.hpp"

namespace Photic
{

class Metronome final
{
public:
    /**
     * Constant used to represent the absence of a time value.
     */
    static const Time_t TIME_NONE;

    /**
     * Creates a Metronome with some period.
     *
     * @param   kPeriod Period in time units. The Metronome will tick every this
     *                  amount of time.
     */
    Metronome (const Time_t kPeriod);

    /**
     * Checks if the Metronome has ticked since the last poll call. The first
     * poll call is always a tick.
     *
     * @param   kTime Current time.
     *
     * @ret     If Metronome has ticked.
     */
    bool poll (const Time_t kTime);

    /**
     * Gets the time elapsed between the previous two ticks, or TIME_NONE if the
     * Metronome has not yet ticked twice.
     *
     * @ret     Dt.
     */
    Time_t getDt () const;

private:
    const Time_t mPeriod; /* Period of Metronome. */
    Time_t       mTLast;  /* Time passed to last poll call. */
    Time_t       mDt;     /* Time elapsed between previous two poll calls. */
};

}; // namespace Photic

#endif