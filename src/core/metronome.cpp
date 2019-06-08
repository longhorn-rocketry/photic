#include "metronome.h"

using namespace photonic;

Metronome::Metronome(float frequency) {
  this->frequency = frequency;
  wavelength = 1 / frequency;
  t_last = -1;
  dt = -1;
}

bool Metronome::poll(float t) {
  if (t_last == -1 || t - t_last >= wavelength) {
    if (t_last != -1)
      dt = t - t_last;
    t_last = t;
    return true;
  }
  return false;
}

float Metronome::get_wavelength() {
  return wavelength;
}

float Metronome::get_dt() {
  return dt;
}
