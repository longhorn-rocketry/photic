#ifndef PHOTONIC_LRA_BMP085_BAROMETER_H
#define PHOTONIC_LRA_BMP085_BAROMETER_H

#ifdef ARDUINO
	#include "Adafruit_BMP085.h"
#endif
#include "barometer.h"

namespace photonic {

/**
	Photonic-friendly wrapper for the Adafruit BMP085
*/
class BMP085Barometer : public Barometer {
protected:
#ifdef ADAFRUIT_BMP085_H
	Adafruit_BMP085 baro;
#endif

public:
	/**
		@brief connect with BMP085
	*/
	bool initialize();

	/**
		@brief updates all sensor readings within the internal BarometerData struct
	*/
	void update();
};

}; // end namespace photonic

#endif
