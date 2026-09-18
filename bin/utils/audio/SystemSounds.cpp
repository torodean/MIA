/**
 * @file SystemSounds.cpp
 * @author Antonius Torode
 * @date 03/09/2021
 * @brief Implements the methods from the associated header file.
 */

// Include the associated header file.
#include "SystemSounds.hpp"

#include <cstdio>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>

// Used for error codes.
#include "Error.hpp"
#include "MIAException.hpp"
// Used for preprocessor definitions.
#include "Constants.hpp"

#if defined(IS_WINDOWS)
	#include <Windows.h>
#endif


namespace system_sounds
{
    bool beep(uint32_t frequency, uint32_t time)
    {
    #if defined(IS_LINUX)
		std::cout << "WARNING: frequency and time are not yet supported"
		          << " in the Beep() method on Linux" 
				  << std::endl;
        int s = open ("/dev/console", O_WRONLY);
        if (s < 0)
		{ // error case.
            perror ("unable to open console");
			return false;
		}
        else
        {
            if (write (s, "\a", 1) != 1)
                perror ("unable to beep");
        }
        std::cout << '\a' << std::flush;
		return true;
    #elif defined(IS_WINDOWS)
		return Beep(frequency, time);
	#else
        MIA_THROW(error::ErrorCode::OS_Not_Supported,
	              "The beep method is only supported on Linux and Windows.");
    #endif
    }
} // namespace system_sounds
