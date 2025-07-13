/**
 * @file Timing.cpp
 * @author Antonius Torode
 * @date 03/09/2021
 * Description: Implements system-level timing utilities for causing the current
 *     thread to sleep for specified durations in milliseconds, seconds (integral 
 *     and fractional), and minutes. Uses C++ standard library facilities to ensure 
 *     cross-platform compatibility.
 */

#include <thread>
#include <chrono>

// Include the associated header file.
#include "Timing.hpp"

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
} // namespace timing
