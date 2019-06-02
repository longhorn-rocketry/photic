#ifndef PHOTONIC_CORE_METRONOME_H
#define PHOTONIC_CORE_METRONOME_H

namespace photonic {

/**
  A class for timing actions at regular intervals.
*/
class Metronome {
protected:
  float frequency, t_last;

public:
  /**
    Creates a new metronome. On construction, the metronome has not started
    ticking, and will start the moment of the first call to poll.

    @param frequency frequency of ticks in Hz
  */
  Metronome(float frequency);

  /**
    Gets if the metronome has ticked. The first poll call is always a tick.

    @param t current time in seconds
  */
  bool poll(float t);
};

}; // end namespace photonic

#endif
