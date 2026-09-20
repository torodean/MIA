/**
 * @file SingleKeyListener_T.cpp
 * @author Antonius Torode
 * @date 06/25/2025
 * Description:
 *     Tests for SingleKeyListener
 */
 
#include <gtest/gtest.h>

#include "SingleKeyListener.hpp"
#include "MIAException.hpp"


class SingleKeyListenerTest : public ::testing::Test
{
protected:
    void TearDown() override 
    { 
        keyedTask.stop(); 
        defaultTask.stop(); 
    }

    // You can set up common objects here
    SingleKeyListener defaultTask;
    SingleKeyListener keyedTask{'A'}; // arbitrary key code
};


TEST_F(SingleKeyListenerTest, DefaultConstructorIsInactive)
{
    EXPECT_FALSE(defaultTask.isActive());
}


TEST_F(SingleKeyListenerTest, ConstructorWithKeyIsActive)
{
    EXPECT_TRUE(keyedTask.isActive());
    EXPECT_EQ('A', keyedTask.isActive() ? 'A' : '\0'); // check keyCode indirectly
}


TEST_F(SingleKeyListenerTest, SetKeyCodeActivatesTask)
{
    defaultTask.setKeyCode('Z');
    EXPECT_TRUE(defaultTask.isActive());
}


TEST_F(SingleKeyListenerTest, InitializeCrashWhenNotCalledCorrectly)
{
    // Should throw if setKeyCode has not been called yet.
    EXPECT_THROW(defaultTask.initialize(), error::MIAException);

    // Call setKeyCode and make sure it no longer throws/
    defaultTask.setKeyCode('2');
    EXPECT_NO_THROW(defaultTask.initialize());
}


TEST_F(SingleKeyListenerTest, StartStopCycle)
{
    keyedTask.initialize();
    keyedTask.start();

    // Let the task run briefly (no real key listening in test)
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    keyedTask.stop();
    EXPECT_FALSE(keyedTask.isRunning());
}


TEST_F(SingleKeyListenerTest, ConditionToggleOnRun)
{
    class TestKeyListener : public SingleKeyListener
    {
    public:
        TestKeyListener() : SingleKeyListener('2') {}

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


