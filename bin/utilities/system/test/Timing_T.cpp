/**
 * File: Timing_T.cpp
 * Author: Antonius Torode
 * Creation Date: 05/20/2025
 * Description: Unit tests for the Timing class, verifying sleep functionality across 
 *    various time units (milliseconds, seconds, minutes) using Google Test. The test
 *    values tested here always allow a small time 'leeway' around what should actually
 *    be returned in order to account for any overhead interactions.
 */

#include <gtest/gtest.h>
#include <chrono>

// Include the associated headers we are testing.
#include "Timing.hpp"

using namespace system;

TEST(TimingTest, SleepMilliseconds) 
{
    auto start = std::chrono::steady_clock::now();
    Timing::sleepMilliseconds(100);
    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    EXPECT_GE(elapsed, 100);
    EXPECT_LT(elapsed, 110);
}

TEST(TimingTest, SleepSecondsInt) 
{
    auto start = std::chrono::steady_clock::now();
    Timing::sleepSeconds(1);
    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    EXPECT_GE(elapsed, 1000);
    EXPECT_LT(elapsed, 1100);
}

TEST(TimingTest, SleepSecondsDouble) 
{
    auto start = std::chrono::steady_clock::now();
    Timing::sleepSeconds(0.2);
    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    EXPECT_GE(elapsed, 200);
    EXPECT_LT(elapsed, 210);
}

TEST(TimingTest, SleepMinutesZero) 
{
    auto start = std::chrono::steady_clock::now();
    Timing::sleepMinutes(0);  // Should return immediately
    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    EXPECT_LT(elapsed, 50);
}
