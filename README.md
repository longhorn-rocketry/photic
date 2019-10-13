![photic](https://stefandebruyn.github.io/assets/images/photic-banner.png)

Photic was created in the interest of building good flight computers and building them fast.

Rewriting entire computers (or even tweaking existing ones) from vehicle to vehicle creates bottlenecks in a rocket's development cycle and increases chances of software error. Instead, we choose to amalgamate all of our flight-proven systems into a single library to guide current and future vehicles reliably.

This library interfaces best with Arduino microcontrollers, but is abstract enough that it can be applied in any C++ stack.

---

### Launch Day is Tomorrow, We Have No Software and I'm Not Lars Blackmore

Not a problem. Import all of Photic with a single line:

```cpp
#include <photic.h>
```

Then, tell Photic a little bit about your rocket.

```cpp
photic::config(ROCKET_MICROCONTROLLER_MODEL, TEENSY_31);
photic::config(ROCKET_IGNITION_G_TRIGGER, 3.0); // >3 Gs = we're on our way
photic::config(ROCKET_NO_IGNITION_GRACE_PERIOD, 60.0 * 10); // Minimum 10 min before liftoff
```

Interfacing your sensors with Photic is as simple as overriding the `initialize` and `update` methods of a particular abstract sensor class. For certain common sensors, such as the Adafruit BMP085, Photic already has an implementation.

```cpp
photic::Imu* imu = new YourCustomImuWrapper();
photic::Barometer* barometer = new photic::BMP085Barometer();

photic::config(ROCKET_PRIMARY_IMU, imu);
photic::config(ROCKET_PRIMARY_BAROMETER, barometer);
```

With setup complete, the computer should block until liftoff, which Photic
is now configured to detect.

```cpp
photic::wait_for_liftoff();
```

There are many ways to do liftoff detection beyond this simple one-liner.
Here, we block until a rolling average of accelerometer readings passes the G
trigger (and the no-ignition grace period is enforced automatically).

```cpp
photic::history<float> vertical_accel_history(10); // Rolling average of 10 values
photic::config(ROCKET_VERTICAL_ACCEL_HISTORY, &vertical_accel_history);

do {
	imu->update();
	vertical_accel_history.add(imu->get_acc_z());
} while (!photic::check_for_liftoff());
```

Once we're in the air, we'll want to record telemetry...

```cpp
photic::TelemetryHeap heap; // Memory IO is known from ROCKET_MICROCONTROLLER_MODEL
photic::block_id_t altitude_data_id = heap.add_block(1024); // Block with 1 kB of storage

barometer->update();
// logc = log compressed; float32 -> float16 for twice the storage density!
heap.logc(altitude_data_id, barometer->get_altitude());
```

...and after recovery, we'll need to retrieve it.

```cpp
float* altitude_data = heap.decompress(altitude_data_id);
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
git clone https://github.com/longhorn-rocketry/photic.git
```

Alternatively, zip the repository and import it in Arduino IDE via `Sketch > Include Library > Add .ZIP Library`.

---

### Maintainers

Maintained by the Experimental branch of the [Longhorn Rocketry Association](http://www.longhornrocketry.org/) at the University of Texas at Austin.
