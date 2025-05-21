/**
 * File: Timing.hpp
 * Author: Antonius Torode
 * Creation Date: 03/09/2021
 * Description: Declares system-level timing utilities intended to provide a unified 
 *    interface for time-based operations such as delays, timing measurements, and 
 *    duration conversions. This header is part of the system utilities module and 
 *    may be extended with additional timing-related functionality.
 */
#pragma once

namespace MIA_system
{    
    /**
     * Causes the current thread to sleep for some number of milliseconds.
     * @param time[int] - Time to sleep in milliseconds.
     */
    static void sleepMilliseconds(int time);

    /**
     * Causes the current thread to sleep for some number of minutes.
     * @param time[int] - Time to sleep in minutes.
     */
    static void sleepMinutes(int time);

    /**
     * Causes the current thread to sleep for some number of seconds.
     * @param time[int] - Time to sleep in seconds.
     */
    static void sleepSeconds(int time);

    /**
     * Causes the current thread to sleep for some number of fractional seconds.
     * @param time[double] - Time to sleep in fractional seconds.
     */
    static void sleepSeconds(double time);

} // namespace MIA_system
