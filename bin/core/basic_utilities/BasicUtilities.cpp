/**
 * File: BasicUtilities.cpp
 * Author: Antonius Torode
 * Created on: 05/22/2025
 * Description: Implements basic utility functions declared in BasicUtilities.hpp.
 */

#include "BasicUtilities.hpp"

namespace BasicUtilities
{
    int findCharInString(const std::string& str, char ch)
    {
        auto pos = str.find(ch);
        return pos == std::string::npos ? -1 : static_cast<int>(pos);
    }
}
