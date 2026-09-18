/**
 * @file MathUtils.hpp
 * @author Antonius Torode
 * @date 03/01/2021
 * @brief Provides general-purpose mathematical utility functions such as random
 *        number generation and dice rolling.
 */
#pragma once

#include <random>

namespace math
{
    /**
     * @brief Generates a random integer between two values (inclusive).
     *
     * @param min Lower bound value to use.
     * @param max Higher bound to use.
     * @param seed A seed to randomize output.
     * @param useTime Mixes the current time into the seed so consecutive calls
     *        differ (default = true).
     * @param verboseMode Enables verbose output (default = false).
     * @return An appropriate random integer.
     */
    int randomInt(int min, 
                  int max, 
                  int seed = std::random_device{}(), 
                  bool useTime = true, 
                  bool verboseMode = false);

    /**
     * @brief Rolls a dice based on input.
     *
     * @param input Input string. Must be of the form #d#, #d##, ##d##, etc.
     * @param verboseMode Enables verbose output (default = false).
     * @return The total dice roll.
     */
    int roll(const std::string &input, bool verboseMode = false);

    /**
     * @brief Returns a 1dXX dice roll total output.
     *
     * @param xx Size of dice.
     * @param seed A seed to use for randomization.
     * @return The total.
     */
    int rolldXX(int xx, int seed);
} // namespace math
