/**
 * File: MathUtils.hpp
 * Author: Antonius Torode
 * Created on: 03/01/2021
 * Description: This file provides a collection of general-purpose mathematical utility functions 
 *     for various common operations such as random number generation, dice rolling, and other 
 *     math-related tasks. It serves as a centralized place for reusable math helpers 
 *     across different projects or modules.
 */
#pragma once

namespace math
{
    /**
     * Generates a random integer between two values.
     * @param min[int] - Lower bound value to use.
     * @param max[int] - Higher bound to use.
     * @param seed[int] - A seed to randomize output.
     * @param useTime[bool] - Uses teh time to determine the seed (default = true).
     * @param verboseMode[bool] - Enables verbose output (default = false).
     * @return [int] - An appropriate random integer.
     */
    int randomInt(int min, int max, int seed, bool useTime = true, bool verboseMode = false);

    /**
     * Rolls a dice based on input.
     * @param input[std::string&] - Input string. Must be of the form #d#, #d##, ##d##, etc.
     * @param verboseMode[bool] - Enables verbose output (default = false).
     * @return [int] - returns the total dice roll.
     */
    int roll(const std::string &input, bool verboseMode = false);

    /**
     * Returns a 1dXX dice roll total output.
     * @param xx[int] - Size of dice.
     * @param seed[int] - A seed to use for randomization.
     * @return [int] returns the total.
     */
    int rolldXX(int xx, int seed);
} // namespace math
