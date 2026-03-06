/**
 * @file AppFramework_T.cpp
 * @author Antonius Torode
 * @date 06/15/2025
 * Description: Tests for AppFramework.hpp to verify AppInterface concept and runApp template behavior.
 */

#include "gtest/gtest.h"
#include "AppFramework.hpp"


// A minimal valid app class fulfilling AppInterface
class ValidApp
{
public:
	// Static values so that they can be checked outside of the class instance.
    static int initializeCalled;
    static int ranCalled;

    void initialize(int argc, char** argv)
    {
        initializeCalled++; // Increment initialized.
        (void)argc; (void)argv; // silence unused parameter warnings
    }

    int run()
    {
        ranCalled++; // Increment ran.
        return 42;
    }
};

// Initialize all static values to 0.
int ValidApp::initializeCalled = 0;
int ValidApp::ranCalled = 0;

// Test that ValidApp satisfies AppInterface and runApp calls initialize and run correctly
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
