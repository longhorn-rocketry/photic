#ifndef PHOTIC_BMP085_BAROMETER_H
#define PHOTIC_BMP085_BAROMETER_H

#ifdef ARDUINO
	#include "Adafruit_BMP085.h"
#endif
#include "barometer.h"

namespace photic {

class BMP085Barometer : public Barometer {
protected:
	bool m_initialized;
#ifdef ADAFRUIT_BMP085_H
	Adafruit_BMP085 m_baro;
#endif

public:
	bool initialize();

	void update();
};

} // end namespace photic

#endif
