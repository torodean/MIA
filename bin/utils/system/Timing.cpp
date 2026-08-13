/**
 * @file Timing.cpp
 * @author Antonius Torode
 * @date 03/09/2021
 * Description: Implements system-level timing utilities for causing the current
 *     thread to sleep for specified durations in milliseconds, seconds (integral 
 *     and fractional), and minutes. Uses C++ standard library facilities to ensure 
 *     cross-platform compatibility.
 */

#include "Timing.hpp"

#include <thread>
#include <chrono>

namespace timing
{
    void sleepMilliseconds(int time)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(time));
    }


    void sleepMinutes(int time)
    {
        std::this_thread::sleep_for(std::chrono::minutes(time));
    }


    void sleepSeconds(int time)
    {
        std::this_thread::sleep_for(std::chrono::seconds(time));
    }


    void sleepSeconds(double time)
    {
        std::this_thread::sleep_for(std::chrono::duration<double>(time));
    }


    bool InterruptableSleeper::sleep(int timeToSleep, timingUnit units)
    {
        return sleep(static_cast<double>(timeToSleep), units);
    }


    bool InterruptableSleeper::sleep(double timeToSleep, timingUnit units)
    {
        std::chrono::duration<double> duration;

        switch(units)
        {
            case timingUnit::time_ms:
                duration = std::chrono::duration<double, std::milli>(timeToSleep);
                break;
            case timingUnit::time_sec:
                duration = std::chrono::duration<double>(timeToSleep);
                break;
            case timingUnit::time_min:
                duration = std::chrono::duration<double, std::ratio<60>>(timeToSleep);
                break;
            case timingUnit::time_hour:
                duration = std::chrono::duration<double, std::ratio<3600>>(timeToSleep);
                break;
            default:
                // TODO - determine default behavior.
                break;
        }

        std::unique_lock lock(mutex);

        const bool wasInterrupted = conditional.wait_for(
            lock,
            duration,
            [this] { return interrupted; }
        );

        interrupted = false;
        return !wasInterrupted;
    }


    void InterruptableSleeper::interruptSleep()
    {
        {
            std::lock_guard lock(mutex);
            interrupted = true;
        }
        conditional.notify_one();
    }
    
    // TODO - add tests for the InterruptableSleeper class.
} // namespace timing
