#ifndef PHOTONIC_SENSOR_BAROMETER_H
#define PHOTONIC_SENSOR_BAROMETER_H

namespace photonic {

/**
	Common barometer readings
*/
struct BarometerData {
	float pressure;
	float altitude;
	float temperature;
};

/**
	Abstract barometer wrapper to be derived by the client and specialized for
	their barometer model and wire mappings.
*/
class Barometer {
protected:
	BarometerData data;

public:
	virtual ~Barometer() = default;

	/**
		@brief setup function, usually a handshake over I2C or serial
	*/
	virtual void initialize() = 0;

	/**
		@brief updates all sensor readings within the internal BarometerData struct
	*/
	virtual void update() = 0;

	/**
		@brief reads all sensor readings into a client's BarometerData struct
	*/
	void read(BarometerData *dest);

	/**
		@brief gets the pressure reading; note: selective read functions such as
		       this and those herein do not place update() calls
	*/
	float get_pressure();

	/**
		@brief gets the altitude reading
	*/
	float get_altitude();

	/**
		@brief gets y acceleration
	*/
	float get_temperature();
};

}; // end namespace photonic

#endif
