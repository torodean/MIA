/**
 * @file CharacterMarkovModel_T.cpp
 * @author Antonius Torode
 * @date 06/05/2025
 * Description:
 *     This file tests the CharacterMarkovModel implementation of the AbstractMarkovModel.
 */
 
#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "CharacterMarkovModel.hpp"
#include "VectorUtils.hpp"

using VectorUtils::unorderedVectorEquals;
using namespace markov_models;

// Test sequences to use as data.
const std::vector<std::string> sequences = 
{ "hello", "fish", "sift", "tilt", "lot" };

// The list of all characters used in the above sequences.
const std::vector<char> allElements =
{ 'e', 'f', 'h', 'i', 'l', 'o', 's', 't' };

TEST(ConstructorTests, probMatrixConstructsProperly) 
{
    // Test default constructor.
    CharacterMarkovModel model;
    model.trainFromStrings(sequences);    
    // The vectors may be out of order, so using unorderedVectorEquals(..).
    EXPECT_TRUE(unorderedVectorEquals<char>(allElements, model.getAllStates()));
    for (const auto& c : allElements)
        EXPECT_TRUE(model.hasState(c));
    
    // Test non-default constructor(s).
    CharacterMarkovModel model2(sequences);
    // The vectors may be out of order, so using unorderedVectorEquals(..).
    EXPECT_TRUE(unorderedVectorEquals<char>(allElements, model2.getAllStates()));
    for (const auto& c : allElements)
        EXPECT_TRUE(model2.hasState(c));
}
