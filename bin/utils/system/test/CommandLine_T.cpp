/**
 * @file CommandLine_T.cpp
 * @author Antonius Torode
 * @date 11/23/2026
 * @brief Unit tests for the command line command runner.
 */

#include <string>

#include <gtest/gtest.h>

// Include the associated file for testing.
#include "CommandLine.hpp"

namespace command_line
{
    /**
     * @brief Verifies the default state of an empty CommandData struct.
     */
    TEST(CommandDataTest, HasSensibleDefaults)
    {
        const CommandData data;
        EXPECT_EQ(data.command, "");
        EXPECT_EQ(data.exitCode, -1);
        EXPECT_EQ(data.stdout, "");
        EXPECT_EQ(data.stderr, "");
        EXPECT_FALSE(data.isSuccess());
    }


    /**
     * @brief Verifies that a successful command returns its command string, exit code,
     *        and standard output.
     */
    TEST(CommandTest, CapturesStdoutAndExitCode)
    {
        const CommandData data = command("echo MIA_stdout_test");

        EXPECT_EQ(data.command, "echo MIA_stdout_test");
        EXPECT_EQ(data.exitCode, 0);
        EXPECT_TRUE(data.isSuccess());
        EXPECT_EQ(data.stdout, "MIA_stdout_test\n");
        EXPECT_EQ(data.stderr, "");
    }


    /**
     * @brief Verifies that a command producing both streams captures them separately.
     */
    TEST(CommandTest, SeparatesStdoutAndStderr)
    {
        const CommandData data = command("echo MIA_stdout_test && echo MIA_stderr_test 1>&2");

        EXPECT_EQ(data.exitCode, 0);
        EXPECT_TRUE(data.isSuccess());
        EXPECT_EQ(data.stdout, "MIA_stdout_test\n");
        EXPECT_EQ(data.stderr, "MIA_stderr_test\n");
    }


    /**
     * @brief Verifies that a non-zero exit code is reported with a false success flag.
     */
    TEST(CommandTest, ReportsNonZeroExitCode)
    {
        const CommandData data = command("exit 4");

        EXPECT_EQ(data.exitCode, 4);
        EXPECT_FALSE(data.isSuccess());
        EXPECT_EQ(data.stdout, "");
        EXPECT_EQ(data.stderr, "");
    }


    /**
     * @brief Verifies that commands run through a shell, so chaining, pipes, and
     *        quoting work.
     */
    TEST(CommandTest, SupportsShellSyntax)
    {
        const CommandData chained = command("echo one && echo two");
        EXPECT_EQ(chained.exitCode, 0);
        EXPECT_EQ(chained.stdout, "one\ntwo\n");

        const CommandData piped = command("printf 'b\\na\\n' | sort");
        EXPECT_EQ(piped.exitCode, 0);
        EXPECT_EQ(piped.stdout, "a\nb\n");

        const CommandData quoted = command("printf '%s\\n' 'hello world'");
        EXPECT_EQ(quoted.exitCode, 0);
        EXPECT_EQ(quoted.stdout, "hello world\n");
    }


    /**
     * @brief Verifies that an empty command succeeds with no output.
     */
    TEST(CommandTest, HandlesEmptyCommand)
    {
        const CommandData data = command("");

        EXPECT_EQ(data.exitCode, 0);
        EXPECT_TRUE(data.isSuccess());
        EXPECT_EQ(data.stdout, "");
        EXPECT_EQ(data.stderr, "");
    }


    /**
     * @brief Verifies that output far larger than the pipe buffer is captured
     *        completely without hanging.
     */
    TEST(CommandTest, CapturesLargeOutputWithoutHanging)
    {
        const CommandData data = command("seq 1 200000");

        EXPECT_EQ(data.exitCode, 0);
        EXPECT_TRUE(data.isSuccess());
        EXPECT_GT(data.stdout.size(), 1000000u);
        EXPECT_EQ(data.stdout.substr(0, 2), "1\n");
        EXPECT_EQ(data.stdout.substr(data.stdout.size() - 7), "200000\n");
    }
} // namespace command_line
