/**
 * @file SystemSounds.cpp
 * @author Antonius Torode
 * @date 03/09/2021
 * Description:
 */

#include <cstdio>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>

//Used for error codes.
#include "Error.hpp"
#include "MIAException.hpp"
// Include the associated header file.
#include "SystemSounds.hpp"

namespace SystemSounds
{
    void beep()
    {
    #if __linux__
        int s = open ("/dev/console", O_WRONLY);
        if (s < 0)
            perror ("unable to open console");
        else
        {
            if (write (s, "\a", 1) != 1)
                perror ("unable to beep");
        }
        std::cout << '\a' << std::flush;
    #else
        throw error::MIAException(error::ErrorCode::Linux_Only_Feature);
    #endif
    }
} // namespace SystemSounds
