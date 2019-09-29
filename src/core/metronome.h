#ifndef PHOTIC_CORE_METRONOME_H
#define PHOTIC_CORE_METRONOME_H

namespace photic {

/**
 * A class for timing actions at regular intervals.
 */
class Metronome {
protected:
  float m_frequency;
  float m_wavelength;
  float m_t_last;
  float m_dt;

public:
  /**
   * Creates a new metronome On construction, the metronome has not started
   * ticking, and will start the moment of the first poll call.
   *
   * @param k_freq metronome frequency (Hz)
   */
  Metronome(float k_freq);

  /**
   * Gets if the metronome has ticked. The first poll call is always a tick.
   *
   * @param k_t current time in seconds
   */
  bool poll(float k_t);

  /**
   * Gets the wavelength of the metronome.
   *
   * @return wavelength
   */
  float get_wavelength();

  /**
   * Gets the delta t of the last poll call. If only one poll call has been
   * made, dt is -1.
   *
   * @return dt
   */
  float get_dt();

  /**
   * @brief Revert to initial state, as though the metronome had never been
   *        polled.
   */
  float reset();
};

} // end namespace photic

#endif
