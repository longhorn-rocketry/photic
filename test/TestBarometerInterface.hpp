/**
 * Tests for BarometerInterface.
 */

#ifndef TEST_BAROMETER_INTERFACE_HPP
#define TEST_BAROMETER_INTERFACE_HPP

#include "BarometerInterface.hpp"
#include "TestMacros.hpp"

using namespace Photic;

namespace TestBarometerInterface
{

/**
 * Test BarometerInterface implementation.
 */
class MockBarometerInterface final : public BarometerInterface
{
public:
    MockBarometerInterface () {}

    virtual bool init ()
    {
        return true;
    }

    virtual bool run ()
    {
        mData.pressure = 1;
        mData.temperature = 2;
        mData.altitude = 3;

        return true;
    }
};

/**
 * Tests that BarometerInterface returns sensor data correctly.
 */
void test ()
{
    TEST_DEFINE ("BarometerInterfaceReadings");

    // Check that barometer is initialized and run successfully.
    MockBarometerInterface baro;
    CHECK_TRUE (baro.init ());
    CHECK_TRUE (baro.run ());

    // Check that correct readings are returned.
    CHECK_EQUAL (baro.getPressure (), 1);
    CHECK_EQUAL (baro.getTemperature (), 2);
    CHECK_EQUAL (baro.getAltitude (), 3);
}
    
} // namespace TestBarometerInterface

#endif