#include "Metronome.hpp"

namespace Photic
{

const Time_t Metronome::TIME_NONE = -1;

Metronome::Metronome (const Time_t kPeriod) : mPeriod (kPeriod),
                                              mTLast  (TIME_NONE),
                                              mDt     (TIME_NONE) {}

bool Metronome::poll (const Time_t kTime)
{
    // Metronome has ticked since last call.
    if (mTLast == TIME_NONE || kTime - mTLast >= mPeriod)
    {
        // If this is not the first tick, a dt can be calculated.
        if (mTLast != TIME_NONE)
        {
            mDt = kTime - mTLast;
        }

        mTLast = kTime;
        return true;
    }

    // Metronome has not ticked since last call.
    return false;
}

Time_t Metronome::getDt () const
{
    return mDt;
}

} // namespace Photic