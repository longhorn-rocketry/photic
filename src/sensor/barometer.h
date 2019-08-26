#ifndef PHOTIC_SENSOR_BAROMETER_H
#define PHOTIC_SENSOR_BAROMETER_H

namespace photic {

/**
	Common barometer readings
*/
struct BarometerData {
	float pressure;
	float altitude;
	float temperature;
};

/*
 * An abstract barometer wrapper to be derived by the client and specialized for
 * their barometer model and wire mappings.
 */
class Barometer {
protected:
	BarometerData m_data;

public:
	virtual ~Barometer() = default;

	/**
	 * @brief One-time setup, usually a handshake.
	 */
	virtual bool initialize() = 0;

	/**
	 * @brief Updates all sensor readings in the internal BarometerData struct.
	 */
	virtual void update() = 0;

	/**
	 * @brief Reads all sensor readings into another BarometerData struct.
	 */
	void read(BarometerData& k_dest);

	/**
	 * @brief Gets the last pressure reading.
	 */
	float get_pressure();

	/**
	 * @brief Gets the last altitude reading.
	 */
	float get_altitude();

	/**
	 * @brief Gets the last temperature reading.
	 */
	float get_temperature();
};

} // end namespace photic

#endif
