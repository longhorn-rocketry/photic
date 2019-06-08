#include "BMP085_barometer.h"

using namespace photonic;

void BMP085Barometer::initialize() {
#ifdef ADAFRUIT_BMP085_H
	while (!baro.begin());
#endif
}

void BMP085Barometer::update() {
#ifdef ADAFRUIT_BMP085_H
	data.pressure = baro.readPressure();
  data.temperature = baro.readTemperature();
  data.altitude = baro.readAltitude(data.pressure);
#endif
}
