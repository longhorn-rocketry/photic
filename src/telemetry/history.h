#ifndef PHOTIC_TELEMETRY_HISTORY_H
#define PHOTIC_TELEMETRY_HISTORY_H

#include <math.h>
#include <vector>

namespace photic {

/**
 * A size-restricted, chronological vector of integral types with statistical
 * analysis methods.
 */
template <typename T>
class history {
protected:
	std::vector<T> m_data;
	unsigned int m_size;
	T m_history_mean, m_history_stdev;
	bool m_computed;

	/**
	 * @brief Computes mean and standard deviation of the history.
	 */
	void compute() {
		T sigma_x = 0, sigma_x_squared = 0;
		unsigned int n = m_data.size();
		for (int i = 0; i < n; i++) {
			sigma_x += m_data[i];
			sigma_x_squared += m_data[i] * m_data[i];
		}
		m_history_mean = sigma_x / n;
		m_history_stdev = n < 2 ? 0 :
				              sqrt((sigma_x_squared - sigma_x * sigma_x / n) / (n - 1));
		m_computed = true;
	}

public:
	/**
	 * @brief Creates an empty history of maximum size k_size.
	 */
	history(unsigned int k_size) {
		m_size = k_size;
		m_data.reserve(k_size);
		m_computed = false;
	}

	/**
	 * @brief Gets the mean of the history contents.
	 */
	T mean() {
		if (!m_computed)
			compute();

		return m_history_mean;
	}

	/**
	 * @brief Gets the standard deviation of the history contents.
	 */
	T stdev() {
		if (!m_computed)
			compute();

		return m_history_stdev;
	}

	/**
	 * @brief Allows access via bracket indexing.
	 */
	T operator[](unsigned int k_i) const {
		return m_data[k_i];
	}

	/**
	 * @brief Allows mutation via bracket indexing.
	 */
  T& operator[](unsigned int k_i) {
		m_computed = false;

		return m_data[k_i];
	}

	/**
	 * Adds a new value to the history. If the history is at capacity, the oldest
	 * value is thrown out.
	 *
	 * @param   k_t value to add
	 */
	void add(T k_t) {
		if (m_data.size() == m_size)
			m_data.erase(m_data.begin());

		m_data.push_back(k_t);

		m_computed = false;
	}

	/**
	 * @brief Gets whether or not the history is at capacity.
	 */
	bool at_capacity() {
		return m_data.size() == m_size;
	}

	/**
	 * @brief Gets the number of items currently in the history
	 *        (at_capacity() -> get_index() == initial_size).
	 */
	int get_index() {
		return m_data.size();
	}

  /**
   * @brief Clears the history.
   */
  void clear() {
    m_data.clear();
  }
};

} // end namespace photic

#endif
