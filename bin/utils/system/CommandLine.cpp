/**
 * @file CommandLine.cpp
 * @author Antonius Torode
 * @date 11/23/2026
 * @brief Implements the utilities for running commands in the command line.
 */

// Include the associated header file.
#include "CommandLine.hpp"

// Used for the platform macros.
#include "Constants.hpp"

#if defined(IS_WINDOWS)
    // TODO - Add windows support.
#elif defined(IS_LINUX)
    // Used for fork, execl, _exit, waitpid, the wait status macros, and the pipe function.
    #include <sys/wait.h>
    #include <unistd.h>
    #include <vector>

    // Used for the pipe reading utilities.
    #include "PipeReader.hpp"
#endif

// Used for exception and error handling.
#include "Error.hpp"
#include "MIAException.hpp"

namespace command_line
{
    CommandData command(const std::string& inputCommand)
    {
#if defined(IS_WINDOWS)
        // TODO - Add windows support.
        MIA_THROW(error::ErrorCode::Feature_In_Dev,
                  "Command line features are not yet implemented for Windows!");
#else
        CommandData data;
        data.command = inputCommand;

        int stdoutPipe[2];
        int stderrPipe[2];
        if (pipe(stdoutPipe) != 0 || pipe(stderrPipe) != 0)
        {
            MIA_THROW(error::ErrorCode::Command_Failed_To_Start,
                      "Failed to create pipes for command: " + inputCommand);
        }

        const pid_t pid = fork();
        if (pid < 0)
        {
            close(stdoutPipe[0]);
            close(stdoutPipe[1]);
            close(stderrPipe[0]);
            close(stderrPipe[1]);
            MIA_THROW(error::ErrorCode::Command_Failed_To_Start,
                      "Failed to start command: " + inputCommand);
        }
        if (pid == 0)
        {
            dup2(stdoutPipe[1], STDOUT_FILENO);
            dup2(stderrPipe[1], STDERR_FILENO);
            close(stdoutPipe[0]);
            close(stdoutPipe[1]);
            close(stderrPipe[0]);
            close(stderrPipe[1]);
            execl("/bin/sh", "sh", "-c", inputCommand.c_str(), static_cast<char*>(nullptr));
            // Only reached if execl failed; 127 is the standard "command not found" exit code.
            _exit(127);
        }

        // The parent keeps the read ends and closes the write ends so the pipes close once the child exits.
        close(stdoutPipe[1]);
        close(stderrPipe[1]);
        const std::vector<std::string> pipeOutputs = command_line::readPipes({stdoutPipe[0], stderrPipe[0]});
        data.stdout = pipeOutputs[0];
        data.stderr = pipeOutputs[1];
        close(stdoutPipe[0]);
        close(stderrPipe[0]);

        int status = 0;
        if (waitpid(pid, &status, 0) < 0)
        {
            MIA_THROW(error::ErrorCode::Command_Failed_To_Start,
                      "Failed to wait for command: " + inputCommand);
        }
        // A negative exit code reports a process which ended without one (e.g., by signal).
        data.exitCode = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
        return data;
#endif
    }
} // namespace command_line
