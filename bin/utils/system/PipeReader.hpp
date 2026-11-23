/**
 * @file PipeReader.hpp
 * @author Antonius Torode
 * @date 11/23/2026
 * @brief Contains POSIX utilities for reading data out of pipe file descriptors.
 */
#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace command_line
{
    // The default number of bytes read at a time from a pipe.
    inline constexpr std::size_t defaultOutputBufferSize = 4096;

    /**
     * @brief Reads one buffer of data from a pipe.
     *
     * Reads only one buffer per call; readPipes relies on this to empty several
     * pipes without blocking.
     *
     * @param fileDescriptor The ready pipe file descriptor to read from.
     * @param bufferSize Maximum bytes to read in one call (default = defaultOutputBufferSize).
     * @return The data read from the pipe, or an empty string if the writer closed the pipe.
     * @throws error::MIAException if a read error occurs or the buffer size is zero.
     */
    std::string readReadyPipe(int fileDescriptor,
                              std::size_t bufferSize = defaultOutputBufferSize);

    /**
     * @brief Reads data from a pipe until the writer closes it.
     *
     * @param fileDescriptor The pipe file descriptor to read from.
     * @param bufferSize Maximum bytes to read in one call (default = defaultOutputBufferSize).
     * @return The data read from the pipe.
     * @throws error::MIAException if a read error occurs or the buffer size is zero.
     * @note The file descriptor is left open; the caller is responsible for closing it.
     */
    std::string readPipeOutput(int fileDescriptor,
                               std::size_t bufferSize = defaultOutputBufferSize);

    /**
     * @brief Reads from any number of pipes until all are closed by their writers.
     *
     * Pipes are emptied alternately with select, so a full buffer on one pipe does not
     * block its writer while the others are read; reading each pipe to completion could
     * hang forever on large output.
     *
     * @param fileDescriptors The pipe file descriptors to read from.
     * @param bufferSize Maximum bytes to read at a time from each pipe (default = defaultOutputBufferSize).
     * @return The data read from each pipe, in the same order as the file descriptors.
     * @throws error::MIAException if a read or select error occurs, or the buffer size is zero.
     * @note The file descriptors are left open; the caller is responsible for closing them.
     */
    std::vector<std::string> readPipes(const std::vector<int>& fileDescriptors,
                                       std::size_t bufferSize = defaultOutputBufferSize);
} // namespace command_line
