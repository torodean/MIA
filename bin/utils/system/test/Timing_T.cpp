/**
 * @file Timing_T.cpp
 * @author Antonius Torode
 * @date 05/20/2025
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

/// The max number of times to attempt the tests if they fail.
static constexpr int maxAttempts = 5;

TEST(TimingTest, SleepMilliseconds) 
{
    for (int attempt = 1; attempt <= maxAttempts; ++attempt)
    {
		auto start = std::chrono::steady_clock::now();
		sleepMilliseconds(100);
		auto end = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();


        if (elapsed >= 95 && elapsed < 115) // Windows requires this to be rather long to succeed.
            return;  // success

        if (attempt == maxAttempts)
			FAIL() << "sleepMilliseconds(100) timing out of range after "
				   << maxAttempts << " attempts. "
				   << "Last measured: " << elapsed << " ms. "
				   << "Expected: [95 ms, 115 ms).";
	}
}

TEST(TimingTest, SleepSecondsInt) 
{
    for (int attempt = 1; attempt <= maxAttempts; ++attempt)
    {
		auto start = std::chrono::steady_clock::now();
		sleepSeconds(1);
		auto end = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        if (elapsed >= 995 && elapsed < 1100) // Windows requires this to be rather long to succeed.
            return;  // success

        if (attempt == maxAttempts)
			FAIL() << "sleepSeconds(1) timing out of range after "
				   << maxAttempts << " attempts. "
				   << "Last measured: " << elapsed << " ms. "
				   << "Expected: [995 ms, 1100 ms).";
	}
}

TEST(TimingTest, SleepSecondsDouble) 
{
    for (int attempt = 1; attempt <= maxAttempts; ++attempt)
    {
		auto start = std::chrono::steady_clock::now();
		sleepSeconds(0.2);
		auto end = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        if (elapsed >= 195 && elapsed < 215) // Windows requires this to be rather long to succeed.
            return;  // success

        if (attempt == maxAttempts)
			FAIL() << "sleepSeconds(0.2) timing out of range after "
				   << maxAttempts << " attempts. "
				   << "Last measured: " << elapsed << " ms. "
				   << "Expected: [195 ms, 215 ms).";
	}
}

TEST(TimingTest, SleepMinutesZero) 
{
    auto start = std::chrono::steady_clock::now();
    sleepMinutes(0);  // Should return immediately
    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    EXPECT_LT(elapsed, 50);
}
