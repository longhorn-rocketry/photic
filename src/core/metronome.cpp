#include "metronome.h"

namespace photic {

Metronome::Metronome(float k_freq) {
  m_frequency = k_freq;
  m_wavelength = 1 / k_freq;
  m_t_last = -1;
  m_dt = -1;
}

bool Metronome::poll(float k_t) {
  if (m_t_last == -1 || k_t - m_t_last >= m_wavelength) {
    if (m_t_last != -1)
      m_dt = k_t - m_t_last;
    m_t_last = k_t;
    return true;
  }
  return false;
}

float Metronome::get_wavelength() {
  return m_wavelength;
}

float Metronome::get_dt() {
  return m_dt;
}

float Metronome::reset() {
  m_t_last = -1;
}

} // end namespace photic
