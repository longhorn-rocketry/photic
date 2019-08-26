#include "BMP085_barometer.h"

namespace photic {

bool BMP085Barometer::initialize() {
	m_initialized = false;
#ifdef ADAFRUIT_BMP085_H
	m_initialized = m_baro.begin();
#endif
	return m_initialized;
}

void BMP085Barometer::update() {
#ifdef ADAFRUIT_BMP085_H
	if (m_initialized) {
		m_data.pressure = m_baro.readPressure();
	  m_data.temperature = m_baro.readTemperature();
	  m_data.altitude = m_baro.readAltitude(m_data.pressure);
	}
#endif
}

} // end namespace photic
