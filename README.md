![photonic](assets/images/banner.png)

Photonic was created in the interest of building good flight computers and building them fast.

Rewriting entire computers (or even tweaking existing ones) from vehicle to vehicle creates bottlenecks in a rocket's development cycle and increases chances of software error. Rather, we choose to amalgamate all of our flight-proven systems into a single library to guide current and future vehicles reliably.

This library interfaces best with Arduino microcontrollers, but is abstract enough that it can be applied in any C++ stack.

**Disclaimer: This library is a work-in-progress, and several components are pending flight testing.**

---

### Launch Day is Tomorrow, We Have No Software and I'm Not Lars Blackmore; Send Help

Import all of Photonic with a single line:

```cpp
#include <photonic.h>

using namespace photonic;
```

Then, tell Photonic a little bit about your rocket.

```cpp
photonic_configure(ROCKET_IGNITION_G_TRIGGER, 3.0);
photonic_configure(ROCKET_NO_IGNITION_GRACE_PERIOD, 60.0 * 10);
photonic_configure(ROCKET_MICROCONTROLLER, TEENSY_31);
```

Interfacing your sensors with Photonic is as simple as overriding the `initialize` and `update` methods of a particular abstract sensor class. For certain common sensors, such as the Adafruit BMP085, Photonic already has an implementation.

```cpp
Imu *imu = new YourCustomImuWrapper(); // our IMU is highly specialized
Barometer *baro = new BMP085Barometer(); // we do, however, use the BMP085

imu->initialize();
baro->initialize();

photonic_configure(ROCKET_PRIMARY_IMU, imu);
photonic_configure(ROCKET_PRIMARY_BAROMETER, baro);
```

With setup complete, the computer should delay until liftoff.

```cpp
wait_for_liftoff();
```

Once we're in the air, we'll want to record telemetry...

```cpp
TelemetryHeap heap; // memory IO is understood inherently based on ROCKET_MICROCONTROLLER
int altitude_data_id = heap.add_block(1024); // block with 1 kB of storage

baro->update();
// logc = log compressed; floats get converted to short floats for double the
// storage density!
heap.logc(altitude_data_id, baro->get_altitude());
```

...and after recovery, we'll need to retrieve it.

```cpp
float *altitude_data = heap.decompress(block_id);
int altitude_data_size = heap.get_block_size(altitude_data_id) / 2; // 2 bytes per float16

for (int i = 0; i < altitude_data_size; i++)
	Serial.printf("%f\n", altitude_data[i]);

delete[] altitude_data;
```

You're ready to fly.

---

### Installation

Point your terminal at `Arduino/libraries/` and enter

```
git clone https://github.com/longhorn-rocketry/photonic
```

Alternatively, download this repository as a zip and import it within Arduino IDE via `Sketch > Include Library > Add .ZIP Library`.

---

Maintained by the [Longhorn Rocketry Association](http://www.longhornrocketry.org/) of the University of Texas at Austin.
