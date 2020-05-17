/**
 * Tests for IMUInterface.
 */

#ifndef TEST_IMU_INTERFACE_HPP
#define TEST_IMU_INTERFACE_HPP

#include "IMUInterface.hpp"
#include "TestMacros.hpp"

using namespace Photic;

namespace TestIMUInterface
{

/**
 * Test IMUInterface implementation.
 */
class MockIMUInterface final : public IMUInterface
{
public:
    MockIMUInterface () {}

    virtual bool init ()
    {
        return true;
    }

    virtual bool run ()
    {
        mData.vecAccel[0] = 1;
        mData.vecAccel[1] = 2;
        mData.vecAccel[2] = 3;

        mData.vecMag[0] = 4;
        mData.vecMag[1] = 5;
        mData.vecMag[2] = 6;

        mData.orientEuler[0] = 7;
        mData.orientEuler[1] = 8;
        mData.orientEuler[2] = 9;

        mData.orientQuat[0] = 10;
        mData.orientQuat[1] = 11;
        mData.orientQuat[2] = 12;
        mData.orientQuat[3] = 13;

        return true;
    }
};

/**
 * Tests that IMUInterface returns sensor data correctly.
 */
void testIMUInterfaceReadings ()
{
    TEST_DEFINE ("IMUInterfaceReadings");

    // Check that IMU is initialized and run successfully.
    MockIMUInterface imu;
    CHECK_TRUE (imu.init ());
    CHECK_TRUE (imu.run ());

    // Check that correct acceleration vector is returned.
    Vector3_t vec3 = imu.getAccelerationVector ();
    CHECK_EQUAL (vec3[0], 1);
    CHECK_EQUAL (vec3[1], 2);
    CHECK_EQUAL (vec3[2], 3);

    // Check that correct magnetic field vector is returned.
    vec3 = imu.getMagneticVector ();
    CHECK_EQUAL (vec3[0], 4);
    CHECK_EQUAL (vec3[1], 5);
    CHECK_EQUAL (vec3[2], 6);

    // Check that correct Euler orientation is returned.
    vec3 = imu.getEulerOrientation ();
    CHECK_EQUAL (vec3[0], 7);
    CHECK_EQUAL (vec3[1], 8);
    CHECK_EQUAL (vec3[2], 9);

    // Check that correct quaternion orientation is returned.
    Vector4_t vec4 = imu.getQuaternionOrientation ();
    CHECK_EQUAL (vec4[0], 10);
    CHECK_EQUAL (vec4[1], 11);
    CHECK_EQUAL (vec4[2], 12);
    CHECK_EQUAL (vec4[3], 13);
}

/**
 * Tests that IMUInterface returns sensor data pointers correctly.
 */
void testIMUInterfaceReadingsPtr ()
{
    TEST_DEFINE ("IMUInterfaceReadingsPtr");

    // Check that IMU is initialized and run successfully.
    MockIMUInterface imu;
    CHECK_TRUE (imu.init ());
    CHECK_TRUE (imu.run ());

    // Check that correct acceleration vector is returned.
    Real_t* ptr = imu.getAccelerationVectorPtr ();
    CHECK_EQUAL (ptr[0], 1);
    CHECK_EQUAL (ptr[1], 2);
    CHECK_EQUAL (ptr[2], 3);

    // Check that correct magnetic field vector is returned.
    ptr = imu.getMagneticVectorPtr ();
    CHECK_EQUAL (ptr[0], 4);
    CHECK_EQUAL (ptr[1], 5);
    CHECK_EQUAL (ptr[2], 6);

    // Check that correct Euler orientation is returned.
    ptr = imu.getEulerOrientationPtr ();
    CHECK_EQUAL (ptr[0], 7);
    CHECK_EQUAL (ptr[1], 8);
    CHECK_EQUAL (ptr[2], 9);

    // Check that correct quaternion orientation is returned.
    ptr = imu.getQuaternionOrientationPtr ();
    CHECK_EQUAL (ptr[0], 10);
    CHECK_EQUAL (ptr[1], 11);
    CHECK_EQUAL (ptr[2], 12);
    CHECK_EQUAL (ptr[3], 13);
}

/**
 * Entry point for IMUInterface tests.
 */
void test ()
{
    testIMUInterfaceReadings ();
    testIMUInterfaceReadingsPtr ();
}

} // namespace TestIMUInterface

#endif