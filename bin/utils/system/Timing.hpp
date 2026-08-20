/**
 * @file Timing.hpp
 * @author Antonius Torode
 * @date 03/09/2021
 * Description: Declares system-level timing utilities intended to provide a unified 
 *    interface for time-based operations such as delays, timing measurements, and 
 *    duration conversions. This header is part of the system utilities module and 
 *    may be extended with additional timing-related functionality.
 */
#pragma once

#include <mutex>
#include <condition_variable>

namespace timing
{
    /**
     * This is an enum object defining different timing units.
     */
    enum timingUnit
    {
        time_ms,   ///< millisecond
        time_sec,  ///< second
        time_min,  ///< minute
        time_hour, ///< hour
        time_day   ///< day
    };

    /**
     * Causes the current thread to sleep for some number of milliseconds.
     * @note This is a blocking method.
     * @param time Time to sleep in milliseconds.
     */
    void sleepMilliseconds(int time);

    /**
     * Causes the current thread to sleep for some number of minutes.
     * @note This is a blocking method.
     * @param time Time to sleep in minutes.
     */
    void sleepMinutes(int time);

    /**
     * Causes the current thread to sleep for some number of seconds.
     * @note This is a blocking method.
     * @param time Time to sleep in seconds.
     */
    void sleepSeconds(int time);

    /**
     * Causes the current thread to sleep for some number of fractional seconds.
     * @note This is a blocking method.
     * @param time Time to sleep in fractional seconds.
     */
    void sleepSeconds(double time);

    /**
     * @class InterruptableSleeper
     * @brief Provides an interruptable sleep mechanism.
     *
     * An InterruptableSleeper blocks the calling thread for the requested duration,
     * unless interruptSleep() is called from another thread. The sleep duration
     * and timing unit are specified when calling the sleep methods.
     */
    class InterruptableSleeper
    {
    public:
        /**
         * @brief Constructs an InterruptableSleeper using milliseconds.
         */
        InterruptableSleeper() = default;

        /**
         * @brief Destroys the InterruptableSleeper.
         */
        ~InterruptableSleeper() = default;

        /**
         * @brief Sleeps for the specified duration using the specified timing unit.
         *
         * The calling thread will block until the requested duration has elapsed
         * or until interruptSleep() is called.
         *
         * @param timeToSleep The amount of time to sleep.
         * @param units The timing unit used to interpret timeToSleep.
         * @return true if the full sleep duration elapsed.
         * @return false if the sleep was interrupted.
         * @throw MIAException if units is not a recognized timingUnit.
         */
        bool sleep(int timeToSleep, timingUnit units);
        bool sleep(double timeToSleep, timingUnit units);

        /**
         * @brief Interrupts the current sleep.
         *
         * If the sleeper is currently sleeping, the sleeping thread will be
         * awakened immediately. If the sleeper is not currently sleeping,
         * this function has no effect.
         */
        void interruptSleep();

    private:
        std::mutex mutex;
        std::condition_variable conditional;

        /// Indicates whether the current sleep has been interrupted.
        bool interrupted{false};
    }; // class InterruptableSleeper

} // namespace timing
