#ifndef PHOTONIC_TELEMETRY_HISTORY_H
#define PHOTONIC_TELEMETRY_HISTORY_H

#include <vector>

namespace photonic {

/**
	A size-restricted, chronological vector of sensor readings with statistical
	analysis methods.
*/
template <typename T>
class history {
protected:
	std::vector<T> data;
	unsigned int size;
	T history_mean, history_stdev;
	bool computed;

	/**
		@brief computes mean and standard deviation of the history
	*/
	void compute() {
		T sigma_x = 0, sigma_x_squared = 0;
		int n = data.size();
		for (int i = 0; i < n; i++) {
			sigma_x += data[i];
			sigma_x_squared += data[i] * data[i];
		}
		history_mean = sigma_x / n;
		history_stdev = n < 2 ? 0 :
				sqrt((sigma_x_squared - sigma_x * sigma_x / n) / (n - 1));
		computed = true;
	}

public:
	/**
		Creates an empty history.

		@param size maximum number of readings in history
	*/
	history(unsigned int size) {
		this->size = size;
		data.reserve(size);
		computed = false;
	}

	/**
		@brief gets the mean of history contents
	*/
	T mean() {
		if (!computed)
			compute();
		return history_mean;
	}

	/**
		@brief gets the standard deviation of history contents
	*/
	T stdev() {
		if (!computed)
			compute();
		return history_stdev;
	}

	/**
		@brief allows history access indexing via []
	*/
	T operator[](unsigned int i) const {
		return data[i];
	}

	/**
		@brief allows history mutation indexing via []
	*/
  T& operator[](unsigned int i) {
		return data[i];
	}

	/**
		Adds a new value to the history. If the history is at maximum capacity,
		the oldest value is thrown out.

		@param t value to add
	*/
	void add(T t) {
		if (data.size() == size)
			data.erase(data.begin());
		data.push_back(t);
	}
};

}; // end namespace photonic

#endif
