/**
 * @file AppFramework_T.cpp
 * @author Antonius Torode
 * @date 06/15/2025
 * Description: Tests for AppFramework.hpp to verify AppInterface concept and runApp template behavior.
 */

#include "gtest/gtest.h"
#include "AppFramework.hpp"


/**
 * @brief Minimal test implementation of an application using AppInterface.
 *
 * This class is used in unit tests to verify that the application framework
 * correctly invokes the required lifecycle methods. It implements the
 * initialize() and run() functions expected by runApp().
 *
 * Static counters are used instead of instance members so that method
 * invocations can be verified after runApp() constructs and executes its
 * internal instance.
 */
class ValidApp
{
public:
    static int initializeCalled; ///< Counts how many times initialize() was invoked.
    static int ranCalled;        ///< Counts how many times run() was invoked.

    void initialize(int argc, char** argv)
    {
        initializeCalled++;     // Increment initializeCalled.
        (void)argc; (void)argv; // silence unused parameter warnings
    }

    int run()
    {
        ranCalled++; // Increment ranCalled.
        return 42;
    }
};

// Initialize static counters.
int ValidApp::initializeCalled = 0;
int ValidApp::ranCalled = 0;

/**
 * @test AppFramework.ValidAppRunsCorrectly
 * @brief Verifies that runApp() correctly executes the application lifecycle.
 *
 * The test executes runApp<ValidApp>() and confirms:
 * - initialize() is called exactly once.
 * - run() is called exactly once.
 * - the return value from run() is propagated by runApp().
 *
 * Static counters in ValidApp are used to observe method calls because
 * runApp() constructs and manages its own instance internally.
 */
TEST(AppFramework, ValidAppRunsCorrectly)
{
    int argc = 0;
    char** argv = nullptr;

    // Test runApp's return value for correctness.
    int ret = runApp<ValidApp>(argc, argv);
    EXPECT_EQ(ret, 42);
	
	// Check if runApp called the appropriate methods the appropriate number of times.
	EXPECT_EQ(ValidApp::initializeCalled, 1);
    EXPECT_EQ(ValidApp::ranCalled, 1);
}
