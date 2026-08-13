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

namespace timing
{
    /// The max number of times to attempt the tests if they fail.
    static constexpr int maxAttempts = 5;

    /**
     * @brief Tests that a sleep function completes within the expected duration range.
     *
     * The sleep function is attempted up to maxAttempts times to account for
     * cross-platform timing variation. The test succeeds if any attempt completes
     * within the specified range and fails after all attempts fall outside it.
     *
     * @tparam SleepFunction The callable type used to perform the sleep.
     * @param sleepFunction The sleep function to execute and measure.
     * @param minMilliseconds The minimum acceptable elapsed time, inclusive.
     * @param maxMilliseconds The maximum acceptable elapsed time, exclusive.
     * @param description A description of the sleep operation used in the failure message.
     */
    template <typename SleepFunction>
    void expectSleepDuration(
        SleepFunction sleepFunction,
        int minMilliseconds,
        int maxMilliseconds,
        const std::string& description)
    {
        for (int attempt = 1; attempt <= maxAttempts; ++attempt)
        {
            auto start = std::chrono::steady_clock::now();
            sleepFunction();
            auto end = std::chrono::steady_clock::now();

            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

            if (elapsed >= minMilliseconds && elapsed < maxMilliseconds)
                return;

            if (attempt == maxAttempts)
                FAIL() << description
                       << " timing out of range after "
                       << maxAttempts << " attempts. "
                       << "Last measured: " << elapsed << " ms. "
                       << "Expected: [" << minMilliseconds
                       << " ms, " << maxMilliseconds << " ms).";
        }
    }
    

    TEST(TimingTest, SleepMilliseconds) 
    {
        expectSleepDuration([] { sleepMilliseconds(100); }, 95, 115, "sleepMilliseconds(100)");
    }


    TEST(TimingTest, SleepSecondsInt) 
    {
        // Windows requires the end time difference to be rather long to succeed.
        expectSleepDuration([] { sleepSeconds(1); }, 995, 1100, "sleepSeconds(1)");
    }


    TEST(TimingTest, SleepSecondsDouble) 
    {
        // Windows requires the end time difference to be rather long to succeed.
        expectSleepDuration([] { sleepSeconds(0.2); }, 195, 215, "sleepSeconds(0.2)");
    }


    TEST(TimingTest, SleepMinutesZero) 
    {
        auto start = std::chrono::steady_clock::now();
        sleepMinutes(0);  // Should return immediately
        auto end = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        EXPECT_LT(elapsed, 50);
    }

    /**
     * @brief Tests that sleep() from the InterruptableSleeper class sleeps for the requested 
     * number of milliseconds.
     */
    TEST(TimingTest, InterruptableSleepMilliseconds)
    {
        InterruptableSleeper sleeper;
        // The sleep will return true when not interrupted.
        expectSleepDuration([&] { ASSERT_TRUE(sleeper.sleep(100, timingUnit::time_ms)); }, 
                            95, 115, "sleeper.sleep(100, timingUnit::time_ms))");
    }

    /**
     * @brief Tests that sleep() from the InterruptableSleeper class sleeps for the requested 
     * number of seconds.
     */
    TEST(TimingTest, InterruptableSleepSeconds)
    {
        InterruptableSleeper sleeper;
        // Windows requires the end time difference to be rather long to succeed.
        // The sleep will return true when not interrupted.
        expectSleepDuration([&] { ASSERT_TRUE(sleeper.sleep(1, timingUnit::time_sec)); }, 
                            995, 1100, "sleeper.sleep(1, timingUnit::time_sec))");
    }
    
    /**
     * @brief Tests that sleep() from the InterruptableSleeper class sleeps for the requested 
     * number of fractional seconds.
     */
    TEST(TimingTest, InterruptableSleepFractionalSeconds)
    {
        InterruptableSleeper sleeper;
        // The sleep will return true when not interrupted.
        expectSleepDuration([&] { ASSERT_TRUE(sleeper.sleep(0.2, timingUnit::time_sec)); }, 
                            195, 215, "sleeper.sleep(0.2, timingUnit::time_sec))");
    }
    
    /**
     * @brief Tests that sleep() from the InterruptableSleeper class sleeps for the requested 
     * number of fractional hours.
     */
    TEST(TimingTest, InterruptableSleepFractionalHours)
    {
        InterruptableSleeper sleeper;
        // The sleep will return true when not interrupted.
        // 000027778 is about 100 ms in hour units.
        expectSleepDuration([&] { ASSERT_TRUE(sleeper.sleep(0.000027778, timingUnit::time_hour)); }, 
                            95, 115, "sleeper.sleep(0.2, timingUnit::time_sec))");
    }
    
    /**
     * @brief Tests that sleep() from the InterruptableSleeper class returns immediately for 
     * a zero duration.
     */
    TEST(TimingTest, InterruptableSleepZero)
    {
        InterruptableSleeper sleeper;

        auto start = std::chrono::steady_clock::now();
        bool wasNotInterrupted = sleeper.sleep(0, timingUnit::time_ms);
        auto end = std::chrono::steady_clock::now();

        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        EXPECT_TRUE(wasNotInterrupted);
        EXPECT_LT(elapsed, 50);
    }
    
    /**
     * @brief Tests that sleep() from the InterruptableSleeper class can be interrupted for
     * an integer duration.
     */
    TEST(TimingTest, InterruptableSleepInterrupted)
    {
        InterruptableSleeper sleeper;

        std::atomic<bool> wasNotInterrupted{true};

        std::thread sleepingThread([&]
        {
            wasNotInterrupted = sleeper.sleep(1, timingUnit::time_sec);
        });

        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        auto start = std::chrono::steady_clock::now();
        sleeper.interruptSleep();
        sleepingThread.join();
        auto end = std::chrono::steady_clock::now();

        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        EXPECT_FALSE(wasNotInterrupted)
            << "The sleep method of the InterruptableSleeper should have been interrupted.";
        EXPECT_LT(elapsed, 75)
            << "The total elapsed time should not be much more than 50ms.";
    }
    
    /**
     * @brief Tests that sleep() from the InterruptableSleeper class can be interrupted for
     * an double duration.
     */
    TEST(TimingTest, InterruptableSleepFractionalInterrupted)
    {
        InterruptableSleeper sleeper;

        std::atomic<bool> wasNotInterrupted{true};

        std::thread sleepingThread([&]
        {
            wasNotInterrupted = sleeper.sleep(0.9, timingUnit::time_sec);
        });

        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        auto start = std::chrono::steady_clock::now();
        sleeper.interruptSleep();
        sleepingThread.join();
        auto end = std::chrono::steady_clock::now();

        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        EXPECT_FALSE(wasNotInterrupted)
            << "The sleep method of the InterruptableSleeper should have been interrupted.";
        EXPECT_LT(elapsed, 75)
            << "The total elapsed time should not be much more than 50ms.";
    }
    
    /**
     * @brief Tests that a sleep following an interruption completes normally.
     */
    TEST(TimingTest, InterruptableSleepAfterInterruption)
    {
        InterruptableSleeper sleeper;

        std::atomic<bool> wasNotInterrupted{true};

        std::thread sleepingThread([&]
        {
            wasNotInterrupted = sleeper.sleep(1, timingUnit::time_sec);
        });

        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        sleeper.interruptSleep();
        sleepingThread.join();

        EXPECT_FALSE(wasNotInterrupted)
            << "The sleep method of the InterruptableSleeper should have been interrupted.";

        EXPECT_TRUE(sleeper.sleep(10, timingUnit::time_ms))
            << "The sleep method of the InterruptableSleeper should return true when not interrupted.";
    }

    // TODO - add tests for the InterruptableSleeper class when interupting.
} // namespace timing
