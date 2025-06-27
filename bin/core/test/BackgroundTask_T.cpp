/**
 * @file: BackgroundTask_T.cpp
 * @author: Antonius Torode
 * @date: 06/25/2025
 * Description:
 *     Tests the BackgroundTask features.
 */

#include <gtest/gtest.h>
#include <atomic>
#include <chrono>
#include <thread>
#include "BackgroundTask.hpp"

using namespace threading;

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
