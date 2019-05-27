#ifndef PHOTONIC_SENSOR_IMU_H
#define PHOTONIC_SENSOR_IMU_H

namespace photonic {

/**
	Common 9 DOF IMU readings
*/
struct ImuData {
	float ax, ay, az; // 3-axis acceleration
	float gx, gy, gz; // 3-axis gyroscopic motion
	float mx, my, mz; // 3-axis magnetometer readings
};

/**
	Abstract IMU wrapper to be derived by the client and specialized for their
	IMU model and wire mappings.
*/
class Imu {
protected:
	ImuData data;

public:
	/**
		@brief tears down the IMU
	*/
	virtual ~Imu() = default;

	/**
		@brief setup function, usually a handshake over I2C or serial
	*/
	virtual void initialize() = 0;

	/**
		@brief updates all sensor readings within the internal ImuData struct
	*/
	virtual void update() = 0;

	/**
		@brief reads all sensor readings into a client's ImuData struct
	*/
	void read(ImuData *dest);

	/**
		@brief gets x acceleration; note: selective read functions such as this
		       and those herein do not place update() calls
	*/
	float get_acc_x();

	/**
		@brief gets y acceleration
	*/
	float get_acc_y();

	/**
		@brief gets z acceleration
	*/
	float get_acc_z();

	/**
		@brief gets x angular acceleration
	*/
	float get_gyro_x();

	/**
		@brief gets y angular acceleration
	*/
	float get_gyro_y();

	/**
		@brief gets z angular acceleration
	*/
	float get_gyro_z();

	/**
		@brief gets x magnetic field strength
	*/
	float get_mag_x();

	/**
		@brief gets y magnetic field strength
	*/
	float get_mag_y();

	/**
		@brief gets z magnetic field strength
	*/
	float get_mag_z();
};

}; // end namespace photonic

#endif
