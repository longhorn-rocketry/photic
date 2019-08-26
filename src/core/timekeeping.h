#ifndef PHOTIC_CORE_TIMEKEEPING_H
#define PHOTIC_CORE_TIMEKEEPING_H

namespace photic {

/**
 * A source of time measurements.
 */
class Timekeeper {
public:
	/**
	 * @brief Tears down the timekeeper.
	 */
	virtual ~Timekeeper() = default;

	/**
	 * @brief Gets the current time. Units and relativity are up to the
	 *        implementor.
	 */
	virtual float time() = 0;
};

/**
 * Timekeeping via Arduino's millis().
 */
class ArduinoTimekeeper : public Timekeeper {
public:
	/**
	 * @brief Gets Arduino time in seconds.
	 */
	float time();
};

} // end namespace photic

#endif
