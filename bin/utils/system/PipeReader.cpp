/**
 * @file PipeReader.cpp
 * @author Antonius Torode
 * @date 11/23/2026
 * @brief Implements POSIX utilities for reading data out of pipes.
 */

// Include the associated header file.
#include "PipeReader.hpp"

#include <cerrno>
#include <cstddef>
#include <cstring>
#include <sys/select.h>
#include <unistd.h>

// Used for exception and error handling.
#include "Error.hpp"
#include "MIAException.hpp"

namespace command_line
{
    std::string readReadyPipe(int fileDescriptor, std::size_t bufferSize)
    {
        if (bufferSize == 0)
        {
            MIA_THROW(error::ErrorCode::Invalid_Parameter,
                      "The pipe buffer size must be greater than zero.");
        }

        std::vector<char> buffer(bufferSize);
        while (true)
        {
            const ssize_t bytesRead = read(fileDescriptor, buffer.data(), bufferSize);
            if (bytesRead > 0)
            {
                return std::string(buffer.data(), static_cast<std::size_t>(bytesRead));
            }
            if (bytesRead == 0)
            { // The writer closed the pipe.
                return "";
            }
            if (errno == EINTR)
            { // Interrupted by a signal; retry.
                continue;
            }
            MIA_THROW(error::ErrorCode::Pipe_Read_Failure,
                      "Failed to read from a pipe: " + std::string(strerror(errno)));
        }
    }


    std::string readPipeOutput(int fileDescriptor, std::size_t bufferSize)
    {
        if (bufferSize == 0)
        {
            MIA_THROW(error::ErrorCode::Invalid_Parameter,
                      "The pipe buffer size must be greater than zero.");
        }

        std::string output;
        while (true)
        {
            const std::string piece = readReadyPipe(fileDescriptor, bufferSize);
            if (piece.empty())
            { // The writer closed the pipe.
                break;
            }
            output += piece;
        }
        return output;
    }


    std::vector<std::string> readPipes(const std::vector<int>& fileDescriptors, std::size_t bufferSize)
    {
        if (bufferSize == 0)
        {
            MIA_THROW(error::ErrorCode::Invalid_Parameter,
                      "The pipe buffer size must be greater than zero.");
        }

        std::vector<std::string> outputs(fileDescriptors.size());
        std::vector<bool> pipeOpen(fileDescriptors.size(), true);
        while (true)
        {
            fd_set readSet;
            FD_ZERO(&readSet);
            int highestFd = -1;
            bool anyOpen = false;
            for (std::size_t i = 0; i < fileDescriptors.size(); ++i)
            {
                if (pipeOpen[i])
                {
                    anyOpen = true;
                    FD_SET(fileDescriptors[i], &readSet);
                    if (fileDescriptors[i] > highestFd)
                    {
                        highestFd = fileDescriptors[i];
                    }
                }
            }
            if (!anyOpen)
            {
                return outputs;
            }
            const int selectResult = select(highestFd + 1, &readSet, nullptr, nullptr, nullptr);
            if (selectResult < 0)
            {
                if (errno == EINTR)
                { // Interrupted by a signal; retry.
                    continue;
                }
                MIA_THROW(error::ErrorCode::Pipe_Read_Failure,
                          "Failed to select on pipes: " + std::string(strerror(errno)));
            }
            for (std::size_t i = 0; i < fileDescriptors.size(); ++i)
            {
                if (pipeOpen[i] && FD_ISSET(fileDescriptors[i], &readSet))
                {
                    const std::string piece = readReadyPipe(fileDescriptors[i], bufferSize);
                    if (piece.empty())
                    {
                        pipeOpen[i] = false;
                    }
                    else
                    {
                        outputs[i] += piece;
                    }
                }
            }
        }
    }
} // namespace command_line
