#include "barometer.h"

namespace photic {

void Barometer::read(BarometerData& k_dest) {
	k_dest.pressure = m_data.pressure;
	k_dest.altitude = m_data.altitude;
	k_dest.temperature = m_data.temperature;
}

float Barometer::get_pressure() {
	return m_data.pressure;
}

float Barometer::get_altitude() {
	return m_data.altitude;
}

float Barometer::get_temperature() {
	return m_data.temperature;
}

} // end namespace photic
