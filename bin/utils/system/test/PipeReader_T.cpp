/**
 * @file PipeReader_T.cpp
 * @author Antonius Torode
 * @date 11/23/2026
 * @brief Unit tests for the PipeReader utilities.
 */

#include <fcntl.h>
#include <unistd.h>

#include <string>
#include <vector>

#include <gtest/gtest.h>

// Include the associated file for testing.
#include "PipeReader.hpp"

// Used for exception and error handling.
#include "Error.hpp"
#include "MIAException.hpp"

namespace command_line
{
    namespace
    {
        /**
         * @brief Creates a pipe, writes the given data into it, and closes the write end.
         *
         * The data must be small enough to fit into the pipe buffer (64KB on Linux),
         * otherwise the write would block with no reader attached.
         *
         * @param data The data to write into the pipe.
         * @return The read end of the pipe, or -1 on failure.
         */
        int makePipeWithData(const std::string& data)
        {
            int pipeFileDescriptors[2];
            if (pipe(pipeFileDescriptors) != 0)
            {
                return -1;
            }
            if (!data.empty() && write(pipeFileDescriptors[1], data.data(), data.size()) < 0)
            {
                close(pipeFileDescriptors[1]);
                close(pipeFileDescriptors[0]);
                return -1;
            }
            close(pipeFileDescriptors[1]);
            return pipeFileDescriptors[0];
        }
    } // namespace


    /**
     * @brief Verifies that readReadyPipe returns the data in the pipe, then an empty
     *        string once there is nothing left, while leaving the file descriptor open.
     */
    TEST(ReadReadyPipeTest, ReturnsDataThenEmptyStringOncePipeIsEmpty)
    {
        const int fileDescriptor = makePipeWithData("pipe data");
        ASSERT_NE(fileDescriptor, -1);

        EXPECT_EQ(readReadyPipe(fileDescriptor), "pipe data");
        EXPECT_EQ(readReadyPipe(fileDescriptor), "");
        // Verifies the file descriptor is still open.
        EXPECT_NE(fcntl(fileDescriptor, F_GETFD), -1);
        close(fileDescriptor);
    }


    /**
     * @brief Verifies that readPipeOutput reads all data until the writer closes the
     *        pipe, whatever the buffer size.
     */
    TEST(ReadPipeOutputTest, ReadsAllDataUntilWriterCloses)
    {
        const int smallPipe = makePipeWithData("first second third");
        ASSERT_NE(smallPipe, -1);
        EXPECT_EQ(readPipeOutput(smallPipe), "first second third");
        close(smallPipe);

        // 32KiB of data read 1024 bytes at a time; the output must be joined across reads.
        const std::string data(32768, 'x');
        const int largePipe = makePipeWithData(data);
        ASSERT_NE(largePipe, -1);
        EXPECT_EQ(readPipeOutput(largePipe, 1024), data);
        close(largePipe);

        const int emptyPipe = makePipeWithData("");
        ASSERT_NE(emptyPipe, -1);
        EXPECT_EQ(readPipeOutput(emptyPipe), "");
        close(emptyPipe);
    }


    /**
     * @brief Verifies that readPipes returns each pipe's data in file descriptor order.
     */
    TEST(ReadPipesTest, ReturnsDataInFileDescriptorOrder)
    {
        const int first = makePipeWithData("alpha");
        const int second = makePipeWithData("beta");
        ASSERT_NE(first, -1);
        ASSERT_NE(second, -1);

        const std::vector<std::string> outputs = readPipes({first, second});
        ASSERT_EQ(outputs.size(), 2);
        EXPECT_EQ(outputs[0], "alpha");
        EXPECT_EQ(outputs[1], "beta");
        close(first);
        close(second);

        const std::vector<std::string> emptyOutputs = readPipes({});
        EXPECT_EQ(emptyOutputs.size(), 0);
    }


    /**
     * @brief Verifies that readPipes empties large pipes completely without hanging or
     *        truncating data, even with a small buffer size.
     */
    TEST(ReadPipesTest, ReadsLargeOutputWithoutHanging)
    {
        const std::string data(60000, 'y');
        const int first = makePipeWithData(data);
        const int second = makePipeWithData(data);
        ASSERT_NE(first, -1);
        ASSERT_NE(second, -1);

        const std::vector<std::string> outputs = readPipes({first, second}, 1024);
        ASSERT_EQ(outputs.size(), 2);
        EXPECT_EQ(outputs[0], data);
        EXPECT_EQ(outputs[1], data);
        close(first);
        close(second);
    }


    /**
     * @brief Verifies that invalid or closed file descriptors throw a Pipe_Read_Failure
     *        MIAException for every reader function.
     *
     * A closed file descriptor makes select fail with EBADF. A file descriptor of -1 is
     * avoided for readPipes because FD_SET(-1) is undefined behavior and a highestFd of 0
     * makes select return immediately without monitoring anything.
     */
    TEST(InvalidInputTest, ThrowsOnInvalidFileDescriptor)
    {
        EXPECT_THROW(readReadyPipe(-1), error::MIAException);
        EXPECT_THROW(readPipeOutput(-1), error::MIAException);

        const int closedFileDescriptor = makePipeWithData("");
        ASSERT_NE(closedFileDescriptor, -1);
        close(closedFileDescriptor);
        EXPECT_THROW(readPipes({closedFileDescriptor}), error::MIAException);
    }


    /**
     * @brief Verifies that a zero buffer size throws an Invalid_Parameter MIAException
     *        for every reader function.
     */
    TEST(InvalidInputTest, ThrowsOnZeroBufferSize)
    {
        const int fileDescriptor = makePipeWithData("");
        ASSERT_NE(fileDescriptor, -1);

        EXPECT_THROW(readReadyPipe(fileDescriptor, 0), error::MIAException);
        EXPECT_THROW(readPipeOutput(fileDescriptor, 0), error::MIAException);
        EXPECT_THROW(readPipes({fileDescriptor}, 0), error::MIAException);
        close(fileDescriptor);
    }
} // namespace command_line
