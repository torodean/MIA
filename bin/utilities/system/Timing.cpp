/**
 * File: SystemUtils.cpp
 * Author: Antonius Torode
 * Created on: 03/09/2021
 * Description: Implements system-level timing utilities for causing the current
 *     thread to sleep for specified durations in milliseconds, seconds (integral 
 *     and fractional), and minutes. Uses C++ standard library facilities to ensure 
 *     cross-platform compatibility.
 */

#include <thread>
#include <chrono>

// Include the associated header file.
#include "Timing.hpp"

void SystemUtils::sleepMilliseconds(int time)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
}

void SystemUtils::sleepMinutes(int time)
{
    std::this_thread::sleep_for(std::chrono::minutes(time));
}

void SystemUtils::sleepSeconds(int time)
{
    std::this_thread::sleep_for(std::chrono::seconds(time));
}

void SystemUtils::sleepSeconds(double time)
{
    std::this_thread::sleep_for(std::chrono::duration<double>(time));
}
