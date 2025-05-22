/**
 * File: BasicApplication.cpp
 * Author: Antonius Torode
 * Creation Date: 05/22/2025
 * Description: Implements the BasicApplication base class methods.
 *              Handles common initialization such as parsing command line
 *              arguments for verbose mode.
 */
 
#include "MIAApplication.hpp"
#include <iostream>
#include <string>

void MIAApplication::initialize(int argc, char* argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "-v" || arg == "--verbose")
        {
            verboseMode = true;
        }
        else if (arg == "-h" || arg == "--help")
        {
            helpRequested = true;
        }
    }

    if (helpRequested)
    {
        printHelp();
        return; // TODO - Signal to caller that help was requested, app should exit
    }
}

void MIAApplication::printHelp() const
{
    std::cout << "Base application options:\n"
              << "  -v, --verbose    Enable verbose output\n"
              << "  -h, --help       Show this help message\n";
}
