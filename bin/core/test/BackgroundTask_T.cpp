/**
 * @file BackgroundTask_T.cpp
 * @author Antonius Torode
 * @date 06/25/2025
 * Description:
 *     Tests the BackgroundTask features.
 */

#include <gtest/gtest.h>
#include <atomic>
#include <chrono>
#include <thread>
#include "BackgroundTask.hpp"

using namespace threading;

/**
 * @brief Test implementation of BackgroundTask used for unit testing.
 *
 * This class provides a minimal implementation of BackgroundTask to test 
 * lifecycle behavior in tests. The run() method increments an atomic
 * counter each time it executes and automatically requests termination after
 * three iterations. A short sleep is introduced to simulate work and allow
 * thread scheduling.
 *
 * The doWhenStopped() override records whether the shutdown callback was
 * invoked, allowing tests to verify that the BackgroundTask termination
 * sequence completes correctly.
 */
class TestTask : public BackgroundTask 
{
public:
    std::atomic<int> runCount{0};

    void run() override 
    {
        if (stopRequested.load())
            return;
            
        ++runCount;
        
        // Stop after running 3 times
        if (runCount >= 3) 
            stopRequested = true;
            
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    bool stoppedCalled = false;
    void doWhenStopped() override 
    {
        stoppedCalled = true;
    }
};

/**
 * @test BackgroundTaskTest.RunsAndStopsCorrectly
 * @brief Verifies that a BackgroundTask executes repeatedly and stops when requested.
 *
 * A TestTask instance is started and allowed to execute its run() method multiple
 * times in a background thread. The test waits until the task has executed at
 * least three iterations, then stops the task. It verifies that the task is no
 * longer running, that run() executed the expected number of times, and that
 * the doWhenStopped() callback was invoked during shutdown.
 */
TEST(BackgroundTaskTest, RunsAndStopsCorrectly) 
{
    TestTask task;
    EXPECT_TRUE(task.isRunning());

    task.start();

    // Wait until run() has been called at least 3 times
    while (task.runCount.load() < 3) 
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    task.stop();

    EXPECT_FALSE(task.isRunning());
    EXPECT_GE(task.runCount.load(), 3);
    EXPECT_TRUE(task.stoppedCalled);
}

/**
 * @test BackgroundTaskTest.ConditionToggle
 * @brief Verifies that the condition flag managed by BackgroundTask can be set,
 *        queried, and toggled correctly.
 *
 * The test confirms that the condition flag initially evaluates to false,
 * becomes true when setConditionMet(true) is called, and alternates correctly
 * when toggleCondition() is invoked.
 */
TEST(BackgroundTaskTest, ConditionToggle) 
{
    TestTask task;
    EXPECT_FALSE(task.isConditionMet());

    task.setConditionMet(true);
    EXPECT_TRUE(task.isConditionMet());

    task.toggleCondition();
    EXPECT_FALSE(task.isConditionMet());

    task.toggleCondition();
    EXPECT_TRUE(task.isConditionMet());
}
