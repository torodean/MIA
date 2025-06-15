/**
 * File: AppFramework_T.cpp
 * Author: Antonius Torode
 * Created on: 06/15/2025
 * Description: Tests for AppFramework.hpp to verify AppInterface concept and runApp template behavior.
 */

#include "gtest/gtest.h"
#include "AppFramework.hpp"

// A minimal valid app class fulfilling AppInterface
class ValidApp
{
public:
    bool initialized = false;
    bool ran = false;

    void initialize(int argc, char** argv)
    {
        initialized = true;
        (void)argc; (void)argv; // silence unused parameter warnings
    }

    int run()
    {
        ran = true;
        return 42;
    }
};


// Test that ValidApp satisfies AppInterface and runApp calls initialize and run correctly
TEST(AppFramework, ValidAppRunsCorrectly)
{
    ValidApp app;
    int argc = 0;
    char** argv = nullptr;

    // runApp creates its own app instance, so we can't check 'app' flags directly,
    // but we can test runApp's return value for correctness
    int ret = runApp<ValidApp>(argc, argv);
    EXPECT_EQ(ret, 42);
}
