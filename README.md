![photic](https://stefandebruyn.github.io/assets/images/photic-banner.png)

> Semper fi, aim high.

Photic is a flight-proven toolbox for building high-power rocket flight computer
software. It was created in the interest of bringing power and elegance
to what is an oft-overlooked aspect of high-power rocket development.

This library conforms to the Arduino library specification, but is abstract
enough that it can fit into any C++ stack.

---

## Features

- [x] 1 DoF `KalmanFilter` fuses sensor data into accurate altitude estimates
- [x] `Matrix` type and templates like `mat3f`, `mat3d`
- [ ] `Vector` type (and `vec3f`, `vec2d`, etc.) operable with `Matrix`
- [ ] `Quaternion` type and spatial orientation maths
- [ ] Utilities for converting linalg objects to/from Adafruit's `imu` namespace
  - First there was light. And then there was the BNO055.
- [x] `rocketmath` module with common formulas and math utilities
- [x] US Standard Atmosphere model
- [x] `Device` interface (a simple HA layer) encourages simulation integration
- [ ] `StateMachine` removes redundant if-else statekeeping logic
- [x] 16-bit float compression increases telemetry storage density
- [x] General convenience objects reduce common flight logic patterns

(Unchecked features are still on the way.)

---

## Flight-Proven

Photic has earned its wings! It performed nominally on the following rockets:

- [Odysseus (Longhorn Rocketry Association, NASA USLI 2020)](https://www.youtube.com/watch?v=fBGkhfvlj6I)

If you use Photic and want to see your vehicle here, reach out to us!

---

## Installation

Point your terminal at `Arduino/libraries/` and enter

```
git clone https://github.com/longhorn-rocketry/photic
```

Alternatively, zip the repository and import it in Arduino IDE via `Sketch > Include Library > Add .ZIP Library...`.

---

## Maintainers

Developed at the University of Texas at Austin by the [Longhorn Rocketry Association](http://www.longhornrocketry.org/).
