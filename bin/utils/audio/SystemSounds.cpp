/**
 * File: SystemSounds.cpp
 * Author: Antonius Torode
 * Date: 03/09/2021
 * Description:
 */

#include <cstdio>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>

//Used for error codes.
#include "Error.hpp"
// Include the associated header file.
#include "SystemSounds.hpp"

void SystemSounds::beep()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    error::returnError(error::ErrorCode::Linux_Only_Feature);
#elif __linux__
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
   //error::returnError();
#endif
}
