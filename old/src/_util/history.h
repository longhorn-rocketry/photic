/**
 * A size-restricted, chronological vector of integral types with statistical
 * analysis methods. Ideal for taking rolling averages of sensor values.
 */

#ifndef PHOTIC_UTIL_HISTORY_H
#define PHOTIC_UTIL_HISTORY_H

#include <math.h>
#include <stdint.h>
#include <vector>

namespace photic
{

template <typename T>
class History
{
protected:
	/**
	 * History capacity.
	 */
	const std::size_t m_size;
	/**
	 * History contents, in order of addition.
	 */
	std::vector<T> m_data;
	/**
	 * Last computed history mean, not necessarily up-to-date.
	 */
	T m_history_mean;
	/**
	 * Last computed history standard deviation, not necessarily up-to-date.
	 * @return [description]
	 */
	T m_history_stdev;
	/**
	 * Whether or not the history analytics are up-to-date.
	 */
	bool m_computed;

	/**
	 * Computes mean and standard deviation of the history. Called whenever
	 * an analysis method is called and the history has been mutated since the
	 * last call to compute().
	 */
	void compute()
	{
		T sigma_x = 0;
		T sigma_x_squared = 0;
		std::size_t n = m_data.size();

		for (std::size_t i = 0; i < n; i++)
		{
			sigma_x += m_data[i];
			sigma_x_squared += m_data[i] * m_data[i];
		}

		m_history_mean = sigma_x / n;
		m_history_stdev = n < 2 ? 0 :
				sqrt((sigma_x_squared - sigma_x * sigma_x / n) / n);
		m_computed = true;
	}

public:
	/**
	 * Creates an empty history.
	 *
	 * @param   k_size Maximum number of items in history.
	 */
	History(std::size_t k_size) : m_size(k_size)
	{
	    m_data.reserve(k_size);
	    m_computed = false;
	}

	/**
	 * Gets the mean of the history.
	 *
	 * @ret     History mean.
	 */
	T mean()
	{
		if (!m_computed)
		{
			this->compute();
		}

		return m_history_mean;
	}

	/**
	 * Gets the standard deviation of the history.
	 *
	 * @ret     History standard deviation.
	 */
	T stdev()
	{
		if (!m_computed)
		{
			this->compute();
		}

		return m_history_stdev;
	}

	/**
	 * Allows access via bracket indexing.
	 *
	 * @param   k_i Index.
	 *
	 * @ret     Item at index k_i.
	 */
	T operator[](std::size_t k_i) const
	{
		return m_data[k_i];
	}

	/**
	 * Allows mutation via bracket indexing.
	 *
	 * @param   k_i Index.
	 *
	 * @ret     Item at index k_i.
	 */
	T& operator[](std::size_t k_i)
	{
		m_computed = false;
		return m_data[k_i];
	}

	/**
	 * Adds a new item to the history. If the history is at capacity, the oldest
	 * value is thrown out.
	 *
	 * @param   k_t Item to add.
	 */
	void add(T k_t)
	{
		if (m_data.size() == m_size)
		{
			m_data.erase(m_data.begin());
		}

		m_data.push_back(k_t);
		m_computed = false;
	}

	/**
	 * Gets whether or not the history is at capacity.
	 *
	 * @ret   If the history is full.
	 */
	bool at_capacity() const
 	{
 	    return m_data.size() == m_size;
 	}

	/**
	 *  Gets the maximum number of items that may occupy the history.
	 *
	 * @ret     History capacity.
	 */
	std::size_t capacity() const
 	{
 	    return m_size;
 	}

	/**
	 *  Gets the number of items currently in the history such that
	 *  size() == initial size -> at_capacity().
	 */
	std::size_t size() const
 	{
 	    return m_data.size();
 	}

	/**
	 * Clears the history.
	 */
	void clear()
	{
	    m_data.clear();
	    m_computed = false;
	}
};

template <typename T>
using history = History<T>;

} // namespace photic

#endif
