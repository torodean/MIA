/**
 * @file SingleKeyListener_T.cpp
 * @author Antonius Torode
 * @date 06/25/2025
 * @brief Tests for the SingleKeyListener class and associated methods.
 */
 
#include <gtest/gtest.h>

#include "SingleKeyListener.hpp"
#include "MIAException.hpp"


namespace ui
{
    class SingleKeyListener_T : public ::testing::Test
    {
    protected:
        void TearDown() override 
        { 
            keyedTask.stop(); 
            defaultTask.stop(); 
        }

        // Set up common objects here.
        SingleKeyListener defaultTask;
        SingleKeyListener keyedTask{'A'}; // Arbitrary key code.
    };


    /**
     * @brief TODO
     */
    TEST_F(SingleKeyListener_T, DefaultConstructorIsInactive)
    {
        EXPECT_FALSE(defaultTask.isActive());
    }


    /**
     * @brief TODO
     */
    TEST_F(SingleKeyListener_T, ConstructorWithKeyIsActive)
    {
        EXPECT_TRUE(keyedTask.isActive());
        EXPECT_EQ('A', keyedTask.isActive() ? 'A' : '\0'); // check keyCode indirectly.
    }


    /**
     * @brief TODO
     */
    TEST_F(SingleKeyListener_T, SetKeyCodeActivatesTask)
    {
        defaultTask.setKeyCode('Z');
        EXPECT_TRUE(defaultTask.isActive());
    }


    /**
     * @brief TODO
     */
    TEST_F(SingleKeyListener_T, InitializeCrashWhenNotCalledCorrectly)
    {
        // Should throw if setKeyCode has not been called yet.
        EXPECT_THROW(defaultTask.initialize(), error::MIAException);

        // Call setKeyCode and make sure it no longer throws/
        defaultTask.setKeyCode('2');
        EXPECT_NO_THROW(defaultTask.initialize());
    }


    /**
     * @brief TODO
     */
    TEST_F(SingleKeyListener_T, StartStopCycle)
    {
        keyedTask.initialize();
        keyedTask.start();

        // Let the task run briefly (no real key listening in test)
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        keyedTask.stop();
        EXPECT_FALSE(keyedTask.isRunning());
    }


    /**
     * @brief TODO
     */
    TEST_F(SingleKeyListener_T, ConditionToggleOnRun)
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
} // namespace ui
