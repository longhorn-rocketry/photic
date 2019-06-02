#include "core/metronome.h"

using namespace photonic;

Metronome::Metronome(float frequency) {
  this->frequency = frequency;
  t_last = -1;
}

bool Metronome::poll(float t) {
  if (t_last == -1 || t - t_last >= 1 / frequency) {
    t_last = t;
    return true;
  }
  return false;
}
