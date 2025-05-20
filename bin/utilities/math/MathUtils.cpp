/**
 * File: MathUtils.cpp
 * Author: Antonius Torode
 * Created on: 03/01/2021
 * Description: Implementation of general-purpose mathematical utility functions declared in MathUtils.hpp.
 *   This file contains definitions for a variety of math-related helper functions, including 
 *   random number generation, dice rolling mechanics, and other common mathematical operations 
 *   used throughout the project.
 */

#include <iostream>
#include <cmath>
#include <limits>

// Include the associated header file.
#include "MathUtils.hpp"

// @TODO - update this header appropriately for new congig/setup.
#include "../../utilities/StringUtils.hpp"

using std::cout;
using std::endl;

namespace math
{
    int randomInt(int min, int max, int seed, bool useTime, bool verboseMode)
    {
        if(max < min)
        {
            if(verboseMode)
                cout << "...Max value smaller than min, adjusting appropriately by returning min." << endl;
            return min;
        }
        
        if(verboseMode)
            cout << "...Calculating random value between " << min << " and " << max << "." << endl;
        
        if(seed == 0)
            srand((unsigned)time(nullptr));
        else if (useTime)
            srand((unsigned)time(nullptr) + seed);
        else 
            srand(seed);
        
        int random = min + (rand() % (max - min + 1));
        if(verboseMode)
            cout << "...random value is " << random << "." << endl;
        return random;
    }
    
    
    int roll(const std::string& input, bool verboseMode)
    {
        int numOfDice = stoi(StringUtils::getBeforeChar(input, 'd'));
        int sizeOfDice = stoi(StringUtils::getAfterChar(input, 'd'));
        int totalRolled = 0;
    
        if (verboseMode)
            cout << endl;
        for(int i=0; i<numOfDice; i++)
        {
            int randCounter = numOfDice;
            totalRolled += rolldXX(sizeOfDice, randCounter);
        }
        if (verboseMode)
            cout << "Total Rolled: " << totalRolled << endl << endl;
        
        return totalRolled;
    }
    
    
    int rolldXX(int xx, int seed)
    {
        int rand = randomInt(1, xx, seed, true);
        cout << "1d" << xx << ": " << rand << endl;
        return rand;
    }
} // namespace math
