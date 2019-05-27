#ifndef PHOTONIC_CORE_TIMEKEEPING_H
#define PHOTONIC_CORE_TIMEKEEPING_H

namespace photonic {

/**
	An agent that produces time measurements.
*/
class Timekeeper {
public:
	/**
		@brief tears down the timekeeper
	*/
	virtual ~Timekeeper() = default;

	/**
		@brief gets the current time in seconds, relative to nothing in particular
	*/
	virtual float time() = 0;
};

/**
	Timekeeping via Arduino's millis()
*/
class ArduinoTimekeeper {
public:
	/**
		@brief gets the current Arduino time in seconds
	*/
	float time();
};

}; // end namespace photonic

#endif
