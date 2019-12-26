/**
 * A class for timing events at regular intervals. Metronomes "tick" at some
 * frequency and are polled with a timestamp. Example use:
 *
 *   ~ Metronome m(100); // 100 Hz
 *   ~ while (true)
 *   ~ {
 *   ~     float t = <CURRENT TIME IN SECONDS>;
 *   ~     if (m.poll(t))
 *   ~     {
 *   ~         // Code to run 100 times per second.
 *   ~     }
 *   ~ }
 */

#ifndef PHOTIC_UTIL_METRONOME_H
#define PHOTIC_UTIL_METRONOME_H

namespace photic
{

class Metronome
{
protected:
    /**
     * Frequency of metronome ticks in Hz.
     */
    const float m_frequency;
    /**
     * Period of metronome ticks in seconds.
     */
    const float m_period;
    /**
     * Time of last tick, or -1 if none.
     */
    float m_t_last;
    /**
     * Time elapsed between the last two polls that ticked, or -1 if none.
     */
    float m_dt;

public:
    /**
     * On construction, the metronome has not started ticking and will start on
     * the first call to poll().
     *
     * @param   k_freq Metronome frequency (Hz).
     */
    Metronome(float k_freq);

    /**
     * Gets if the metronome has ticked. The first call to this method after
     * construction or reset() is always a tick.
     *
     * @param   k_t Current time in seconds.
     */
    bool poll(float k_t);

    /**
     * Gets the period of the metronome, i.e. the amount of time between
     * subsequent ticks.
     *
     * @ret      Period in seconds.
     */
    float period() const;

    /**
     * Gets the time that elapsed between the last two calls to poll(). If only
     * one poll has been made, -1 is returned. If the metronome is being polled
     * at or around its frequency, dt() should approximately equal period().
     *
     * @ret      dt Delta t in seconds.
     */
    float dt() const;

    /**
     * Revert to initial state, as thought the metronome had never been polled.
     */
    void reset();
};

} // namespace photic

#endif
