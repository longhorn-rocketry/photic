#include "sensor/barometer.h"

using namespace photonic;

void Barometer::read(BarometerData *dest) {
	dest->pressure = data.pressure;
	dest->altitude = data.altitude;
	dest->temperature = data.temperature;
}

float Barometer::get_pressure() {
	return data.pressure;
}

float Barometer::get_altitude() {
	return data.altitude;
}

float Barometer::get_temperature() {
	return data.temperature;
}
