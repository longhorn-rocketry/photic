![photic](https://stefandebruyn.github.io/assets/images/photic-banner.png)

## Photic

Photic is a collection of utilities for writing high power rocket flight
computer software. It was created to bring power and elegance to what is an
oft-overlooked aspect of high power rocket development.

Photic caters to low power microprocessors by minimizing its memory footprint
and external library dependencies. This also makes Photic highly portable. It is
particularly suitable for Arduino or other embedded environments which lack a
complete C++ standard library.

---

## Patterns

The utilities in Photic can simplify programming patterns commonly seen in
high power flight software. Several examples are shown below.

### Liftoff Detection

The flight computer must wait for liftoff. Acting on a single accelerometer
reading would be unwise, so the programmer typically takes a rolling average of
acceleration readings over a period of time for more robust liftoff detection.
For this and similar tasks, Photic provides a special capacitated data structure
called a `History`:

```c++
Photic::History<100> accelReadings; // Capacity 100
while (!accelReadings.atCapacity () && accelReadings.getMean () < 30) // ~3 Gs
{
    imu.run ();
    accelReadings.add (imu.getVerticalAcceleration ());
}
```

### Sensor Interfaces

Hardware-in-the-loop and hardware-out-of-the-loop simulation is a powerful tool
for validating software in flight-like conditions on the ground. These
simulations typically require a way of spoofing sensor readings for
consumption by the flight software. Photic enables this with two abstract
interfaces--`IMUInterface` and `BarometerInterface`--which standardize
communication with these sensors.

The programmer can implement different sensor interfaces under this abstraction
and easily swap between them based on whether the software is running in
simulation or production, e.g.

```c++
Photic::IMUInterface* pImu =
#ifdef HARDWARE_IN_THE_LOOP
    new SimulationIMUInterface (); // IMU interfaced with simulation
#else
    new MyIMUInterface ();         // IMU interfaced with physical sensor
#endif
```

### Navigation

The rocket's altitude, velocity, and acceleration at a point in time are useful
for informing control and recovery decisions. However, the rocket's state can
be difficult to track accurately. In an abominable oversimplification of a
complex algorithm comparable only to `scikit-learn`, Photic provides
`RocketTracker`.

`RocketTracker` is a self-calibrating 1-DOF Kalman filter that tracks a rocket's
vertical state throughout flight. It does this with almost zero user input by
accessing the rocket's sensors directly through the aforementioned sensor
interfaces. Usage looks a bit like this:

```c++
using namespace Photic;
...
RocketTracker::Config_t config = RocketTracker::getDefaultConfig ();
config.pImu = new MyIMUInterface ();
config.pBarometer = new MyBarometerInterface ();
RocketTracker tracker (config);
...
// Returns <altitude, vertical velocity, vertical acceleration>
Vector3_t rocketState = tracker.track ();
```

The navigation filter itself is a standalone class `KalmanFilter` which offers
greater configurability if `RocketTracker` feels like too much of a black box.
Usage for each class is thoroughly documented in their respective headers.

The best navigation solution will be highly specialized to the rocket it flies
on. However, most high-power rockets are similar enough that `RocketTracker`
will perform admirably if a good 9-DOF IMU and barometer are available. The
following graph compares a rocket's velocity as estimated by `RocketTracker` and
a StratoLogger, a COTS flight computer (data taken from [this flight](https://github.com/longhorn-rocketry/telemetry-vault/tree/master/sli-odysseus-test-2-8-20)):

<p align="center">
    <img src="https://stefandebruyn.github.io/assets/images/photic-readme-rockettracker-graph.png">
</p>

---

## Other Features

* `History` data structure for efficient sensor reading analysis
* `BarometerInterface` and `IMUInterface` abstract sensor interfaces
* `RocketTracker` self-calibrating Kalman filter navigation utility
* `KalmanFilter` for greater navigation configurability for advanced users
* `Matrix` data structure and supporting `MathUtils` for common GNC math

---

## Arduino Installation

Download this repository as a zip and import it in Arduino IDE via
`Sketch > Include Library > Add .ZIP Library`.

Then, include all of Photic with the following header:

```c++
#include <Photic.hpp>
```


### 
