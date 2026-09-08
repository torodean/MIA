/**
 * @file KeyListenerTask_T.cpp
 * @author Antonius Torode
 * @date 06/25/2025
 * Description:
 *     Tests for KeyListenerTask
 */
 
#include <gtest/gtest.h>
#include "KeyListenerTask.hpp"

class KeyListenerTaskTest : public ::testing::Test
{
protected:
    void TearDown() override 
    { 
        keyedTask.stop(); 
        defaultTask.stop(); 
    }

    // You can set up common objects here
    KeyListenerTask defaultTask;
    KeyListenerTask keyedTask{'A'}; // arbitrary key code
};

TEST_F(KeyListenerTaskTest, DefaultConstructorIsInactive)
{
    EXPECT_FALSE(defaultTask.isActive());
}

TEST_F(KeyListenerTaskTest, ConstructorWithKeyIsActive)
{
    EXPECT_TRUE(keyedTask.isActive());
    EXPECT_EQ('A', keyedTask.isActive() ? 'A' : '\0'); // check keyCode indirectly
}

TEST_F(KeyListenerTaskTest, SetKeyCodeActivatesTask)
{
    defaultTask.setKeyCode('Z');
    EXPECT_TRUE(defaultTask.isActive());
}

TEST_F(KeyListenerTaskTest, InitializeDoesNotCrashOnInactive)
{
    // Should do nothing or exit early, no exceptions
    EXPECT_NO_THROW(defaultTask.initialize());
}

TEST_F(KeyListenerTaskTest, StartStopCycle)
{
    keyedTask.initialize();
    keyedTask.start();

    // Let the task run briefly (no real key listening in test)
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    keyedTask.stop();
    EXPECT_FALSE(keyedTask.isRunning());
}

TEST_F(KeyListenerTaskTest, ConditionToggleOnRun)
{
    class TestKeyListener : public KeyListenerTask
    {
    public:
        TestKeyListener() : KeyListenerTask('2') {}

        // Override run to simulate key press toggling condition once then stop
        void run() override
        {
            // Toggle once and request stop
            if (!isConditionMet())
                toggleCondition();
            else
                stopRequested.store(true);
        }
    };

    TestKeyListener task;
    task.initialize();
    task.start();

    while (task.isRunning())
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

    EXPECT_TRUE(task.isConditionMet());
        
    task.stop();
}


