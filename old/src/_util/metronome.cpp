#include "util/metronome.h"

namespace photic
{

Metronome::Metronome(float k_freq) : m_frequency(k_freq), m_period(1 / k_freq)
{
    m_t_last = -1;
    m_dt = -1;
}

bool Metronome::poll(float k_t)
{
    if (m_t_last == -1 || k_t - m_t_last >= m_period)
    {
        if (m_t_last != -1)
        {
            m_dt = k_t - m_t_last;
        }

        m_t_last = k_t;
        return true;
    }

    return false;
}

float Metronome::period() const
{
    return m_period;
}

float Metronome::dt() const
{
    return m_dt;
}

void Metronome::reset()
{
    m_t_last = m_dt = -1;
}

} // namespace photic
