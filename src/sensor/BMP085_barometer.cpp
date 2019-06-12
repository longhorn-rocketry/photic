#include "BMP085_barometer.h"

using namespace photonic;

bool BMP085Barometer::initialize() {
	initialized = false;
#ifdef ADAFRUIT_BMP085_H
	initialized = baro.begin();
#endif
	return initialized;
}

void BMP085Barometer::update() {
#ifdef ADAFRUIT_BMP085_H
	if (initialized) {
		data.pressure = baro.readPressure();
	  data.temperature = baro.readTemperature();
	  data.altitude = baro.readAltitude(data.pressure);
	}
#endif
}
