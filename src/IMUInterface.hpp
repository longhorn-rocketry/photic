/**
 * An interface for writing 9-DOF IMU device wrappers. Photic provides these
 * interfaces so that sensor drivers can be switched between easily, e.g. to
 * interface flight software with a hardware-out-of-the-loop simulation.
 *
 *                              ---- USAGE ----
 *
 *   (1) Extend IMUInterface into a child class that will act as the driver for
 *       your specific IMU, e.g.
 *       
 *         class BNO055IMU final : public Photic::IMUInterface
 *
 *   (2) Implement the init function. This function performs one-time setup
 *       logic, e.g. connecting to the sensor and setting calibration values.
 *
 *   (3) Implement run function. This function should read the most recent IMU
 *       measurements and place them in the appropriate vectors within
 *       IMUInterface::mData.
 *
 *   (4) In your flight software, call init once and then run periodically in
 *       the flight logic loop. After each run, access the necessary readings
 *       with the getXXX functions.
 *
 *                              ---- NOTES ----
 *
 *   (1) Each getXXX function has a getXXXPtr variant that returns a pointer to
 *       the array underlying the storage matrix. These functions are preferable
 *       when access speed is of the utmost importance and having the IMU data
 *       in vector form is not particularly useful.
 *
 *       In general, the overhead of copying a Matrix object (i.e. using getXXX
 *       rather than getXXXPtr) should be negligible compared to the time spent
 *       communicating with a sensor over wire. This may not ALWAYS be the case,
 *       however, since introducing new memory always has the potential to
 *       cache thrash, especially on very low power microcontrollers.
 */

#ifndef PHOTIC_IMU_INTERFACE_H
#define PHOTIC_IMU_INTERFACE_H

#include "Matrix.hpp"
#include "Types.hpp"

namespace Photic
{

class IMUInterface
{
public:
    /**
     * Common IMU readings.
     */
    typedef struct
    {
        Vector3_t vecAccel;    /* Acceleration vector. */
        Vector3_t vecMag;      /* Magnetic field vector. */
        Vector3_t orientEuler; /* Euler angles orientation. */
        Vector4_t orientQuat;  /* Quaternion orientation. */
    } Data_t;

    /**
     * NOTE: IMU data is initially garbage.
     */
    IMUInterface ();

    /**
     * Performs one-time setup logic for the IMU.
     *
     * @ret     If setup succeeded.
     */
    virtual bool init () = 0;

    /**
     * Collects the most recent measurements from the IMU.
     *
     * @ret     If run succeeded.
     */
    virtual bool run () = 0;

    /**
     * Gets the most recent acceleration vector.
     *
     * @ret     Acceleration vector.
     */
    Vector3_t getAccelerationVector () const;

    /**
     * Gets the most recent magnetic field vector.
     *
     * @ret     Magnetic field vector.
     */
    Vector3_t getMagneticVector () const;

    /**
     * Gets the most recent Euler angles orientation.
     *
     * @ret     Euler angles orientation.
     */
    Vector3_t getEulerOrientation () const;

    /**
     * Gets the most recent quaternion orientation.
     *
     * @ret     Quaternion orientation.
     */
    Vector4_t getQuaternionOrientation () const;

    /**
     * Gets a pointer to the Real_t array underlying the acceleration vector
     * matrix.
     *
     * @ret     Pointer to acceleration components array.
     */
    Real_t* getAccelerationVectorPtr ();

    /**
     * Gets a pointer to the Real_t array underlying the magnetic field vector
     * matrix.
     *
     * @ret     Pointer to magnetic field components array.
     */
    Real_t* getMagneticVectorPtr ();

    /**
     * Gets a pointer to the Real_t array underlying the Euler angles orientation
     * matrix.
     *
     * @ret     Pointer to Euler angles orientation array.
     */
    Real_t* getEulerOrientationPtr ();

    /**
     * Gets a pointer to the Real_t array underlying the quaternion orientation
     * matrix.
     *
     * @ret     Pointer to Euler angles orientation array.
     */
    Real_t* getQuaternionOrientationPtr ();

protected:
    /**
     * Most recent data read from IMU.
     */
    Data_t mData;
};

} // namespace Photic

#endif