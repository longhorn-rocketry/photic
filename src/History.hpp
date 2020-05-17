/**
 * A History is a capacitated list with statistical analysis methods. Histories
 * begin with 0 elements and grow until they are at capacity. Once at capacity,
 * adding a new element will discard the oldest element in the History.
 *
 * Histories are ideal for acting on rolling windows of sensor readings, e.g.
 * detecting liftoff once the average acceleration passes X or computing the
 * variance in barometer readings for usage in a Kalman filter.
 *
 *                              ---- USAGE ----
 *
 *   (1) Create a History. The template parameter indicates its capacity.
 *
 *         Photic::History<10> vertAccelHist;
 *
 *   (2) Add elements to the History.
 *
 *         Photic::Vector3_t accelVec = imu.getAccelerationVector ();
 *         vertAccelHist.add (accelVec[2]);
 *
 *   (3) Act on History statistics. For example, you could wait for liftoff
 *       by blocking until the average acceleration passes a threshold, e.g.
 *
 *         Photic::History<25> vertAccelHist;
 *         while (!vertAccelHist.atCapacity () && vertAccelHist.mean () < 30)
 *         {
 *             imu.run ();
 *             Real_t vertAccel = (imu.getAccelerationVectorPtr ())[2];
 *             vertAccelHist.add (vertAccel);
 *         }
 *
 *                              ---- NOTES ----
 *
 *   (1) History uses a ring buffer as its internal container. The ring buffer
 *       is implemented with a statically sized array and wrapping index. This
 *       is the fastest data structure for the task since History is unordered.
 *
 *   (2) History statistics (currently just mean and standard deviation) are
 *       cached. They are only recomputed when the user tries to access a
 *       statistic and the History has changed since the last statistic
 *       computation. This action will recompute ALL statistics and not just the
 *       requested statistic.
 *
 *   (3) The behavior of a zero capacity History is undefined.
 */

#ifndef PHOTIC_HISTORY_HPP
#define PHOTIC_HISTORY_HPP

#include <math.h>

#include "Types.hpp"

namespace Photic
{

/**
 * History uses a type larger than Dim_t to represent its dimension since
 * a user may reasonably want to maintain histories larger than 255 items. This
 * type can be safely changed to suit the user's needs.
 */
typedef uint16_t HistoryDim_t;

template <HistoryDim_t T_Dim>
class History
{
protected:
    Real_t       mData[T_Dim]; /* Data in history in no particular order. */
    HistoryDim_t mCurrentSize; /* Current number of elements in history. */
    HistoryDim_t mIdx;         /* Index where next history entry will go. */
    Real_t       mMean;        /* Last computed history mean. */
    Real_t       mStdev;       /* Last computed history standard deviation. */
    bool         mDirty;       /* If mMean and mStdev are out of date. */

    /**
     * Computes the mean and standard deviation of the history.
     */
    void computeStats ()
    {
        const HistoryDim_t n = mCurrentSize;
        Real_t sigmaX = 0;
        Real_t sigmaXSqr = 0;

        // For all x in history, compute Sigma(x) and Sigma(x^2).
        for (HistoryDim_t i = 0; i < n; i++)
        {
            sigmaX += mData[i];
            sigmaXSqr += mData[i] * mData[i];
        }

        // Compute mean and stdev.
        mMean = sigmaX / n;
        mStdev = n < 2 ?
                     0 : sqrt ((sigmaXSqr - sigmaX * sigmaX / n) / n);

        // Mark these stats as up to date.
        mDirty = false;
    }

public:
    /**
     * Constructor.
     */
    History () : mCurrentSize (0), mIdx (0), mDirty (true) {}

    /**
     * Adds a new element to the history. If the history is at capacity, the
     * oldest element is thrown out.
     *
     * @param   kData New element.
     */
    void add (Real_t kData)
    {
        mData[mIdx++] = kData;

        // Increment size if not yet at capacity.
        if (mCurrentSize < T_Dim)
        {
            mCurrentSize++;
        }

        // Wrap index around to start of history to replace oldest value on
        // next add call.
        if (mIdx >= T_Dim)
        {
            mIdx = 0;
        }

        // Invalidate last stat computations.
        mDirty = true;
    }

    /**
     * Gets the history mean.
     *
     * @ret     History mean.
     */
    Real_t getMean ()
    {
        if (mDirty)
        {
            this->computeStats ();
        }

        return mMean;
    }

    /**
     * Gets the history standard deviation.
     *
     * @ret     History standard deviation.
     */
    Real_t getStdev ()
    {
        if (mDirty)
        {
            this->computeStats ();
        }

        return mStdev;
    }

    /**
     * Gets if the history is at capacity.
     *
     * @ret     If history is at capacity.
     */
    bool atCapacity () const
    {
        return (mCurrentSize == T_Dim);
    }

    /**
     * Clears all elements from the history.
     */
    void clear ()
    {
        mIdx = 0;
        mCurrentSize = 0;
        mDirty = true;
    }
};

} // namespace Photic

#endif