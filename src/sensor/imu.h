#ifndef PHOTIC_SENSOR_IMU_H
#define PHOTIC_SENSOR_IMU_H

namespace photic {

struct ImuData {
	float ax, ay, az; // 3-axis acceleration
	float gx, gy, gz; // 3-axis gyroscopic motion
	float mx, my, mz; // 3-axis magnetic field strength
};

/**
 * An abstract IMU wrapper to be derived by the client and specialized for
 * their barometer model and wire mappings.
 */
class Imu {
protected:
	ImuData m_data;

public:
	virtual ~Imu() = default;

	/**
	 * @brief One-time setup, usually a handshake.
	 */
	virtual bool initialize() = 0;

	/**
	 * @brief Updates all sensor readings in the internal ImuData struct.
	 */
	virtual void update() = 0;

	/**
	 * @brief Reads all sensor readings into another ImuData struct.
	 */
	void read(ImuData& k_dest);

	/**
	 * @brief Gets the last x acceleration reading.
	 */
	float get_acc_x();

	/**
	 * @brief Gets the last y acceleration reading.
	 */
	float get_acc_y();

	/**
	 * @brief Gets the last z acceleration reading.
	 */
	float get_acc_z();

	/**
	 * @brief Gets the last x gyro reading.
	 */
	float get_gyro_x();

	/**
	 * @brief Gets the last y gyro reading.
	 */
	float get_gyro_y();

	/**
	 * @brief Gets the last z gyro reading.
	 */
	float get_gyro_z();

	/**
	 * @brief Gets the last x magnetometer reading.
	 */
	float get_mag_x();

	/**
	 * @brief Gets the last y magnetometer reading.
	 */
	float get_mag_y();

	/**
	 * @brief Gets the last z magnetometer reading.
	 */
	float get_mag_z();
};

} // end namespace photic

#endif
