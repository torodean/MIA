/**
 * File: BasicUtilities.cpp
 * Author: Antonius Torode
 * Created on: 05/22/2025
 * Description: Implements basic utility functions declared in BasicUtilities.hpp.
 */

#include <string>
#include <algorithm>
#include <cctype>

#include "BasicUtilities.hpp"

namespace BasicUtilities
{
    int findCharInString(const std::string& str, char ch)
    {
        auto pos = str.find(ch);
        return pos == std::string::npos ? -1 : static_cast<int>(pos);
    }
    
    
    std::string lstrip(const std::string& s) 
    {
        auto start = std::find_if_not(s.begin(), s.end(), [](unsigned char ch) 
        {
            return std::isspace(ch);
        });
        return std::string(start, s.end());
    }
    
    
    std::string rstrip(const std::string& s) 
    {
        auto end = std::find_if_not(s.rbegin(), s.rend(), [](unsigned char ch) 
        {
            return std::isspace(ch);
        }).base();
        return std::string(s.begin(), end);
    }
    
    
    std::string strip(const std::string& s) 
    {
        return lstrip(rstrip(s));
    }
}
