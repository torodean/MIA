/**
 * @file CommandLine.hpp
 * @author Antonius Torode
 * @date 11/23/2026
 * @brief Utilities for running commands from the system command line.
 */
#pragma once

#include <string>

/**
 * This namespace is for features which directly interact with the system command line.
 */
namespace command_line
{
    /**
     * @brief The results of a command line command run by command().
     */
    struct CommandData
    {
        /// The command that was run.
        std::string command;

        /// The exit code of the command's process, or -1 if it ended without an exit code.
        int exitCode{-1};

        /// The standard output captured from the command.
        std::string stdout;

        /// The standard error captured from the command.
        std::string stderr;

        /**
         * @brief Returns true if the command's process exited with code 0.
         * @return true if the exit code is 0.
         */
        bool isSuccess() const { return exitCode == 0; }
    };

    /**
     * @brief Runs a command line command in the system shell and waits for it to finish.
     *
     * The command is passed to the system shell (cmd on Windows, sh on Linux), so built-in
     * commands and shell syntax work the same as typed at a prompt. The command's standard
     * output and standard error are captured into the returned CommandData instead of being
     * printed to the application's console.
     *
     * @param inputCommand The command line command to run.
     * @return The results of the command, including its exit code and captured output.
     * @throws error::MIAException if the process could not be started.
     */
    CommandData command(const std::string& inputCommand);
} // namespace command_line
