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

using namespace timing;

TEST(TimingTest, SleepMilliseconds) 
{
    auto start = std::chrono::steady_clock::now();
    sleepMilliseconds(100);
    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    EXPECT_GE(elapsed, 95);
    EXPECT_LT(elapsed, 115); // Windows requires this to be rather long to succeed.
}

TEST(TimingTest, SleepSecondsInt) 
{
    auto start = std::chrono::steady_clock::now();
    sleepSeconds(1);
    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    EXPECT_GE(elapsed, 995);
    EXPECT_LT(elapsed, 1100); // Windows requires this to be rather long to succeed.
}

TEST(TimingTest, SleepSecondsDouble) 
{
    auto start = std::chrono::steady_clock::now();
    sleepSeconds(0.2);
    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    EXPECT_GE(elapsed, 195);
    EXPECT_LT(elapsed, 215); // Windows requires this to be rather long to succeed.
}

TEST(TimingTest, SleepMinutesZero) 
{
    auto start = std::chrono::steady_clock::now();
    sleepMinutes(0);  // Should return immediately
    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    EXPECT_LT(elapsed, 50); 
}
