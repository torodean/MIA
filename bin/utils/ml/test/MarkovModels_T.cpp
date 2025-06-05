/*
 * File: MarkovModels_T.cpp
 * Author: Antonius Torode
 * Created on: 06/05/2025
 * Description: 
 *     This file is for testing the moethods in the MarkovModels file(s).
 */

#include <gtest/gtest.h>
#include "MarkovModels.hpp"

using namespace markov_models;

// Test data setup
const std::vector<std::vector<char>> char_sequences = {
    {'a', 'b', 'a', 'c'},
    {'a', 'b', 'b'},
    {'b', 'a'}
};

TEST(GenerateProbabilityMatrixTest, BasicProbabilityCalculation) 
{
    auto matrix = generate_probability_matrix(char_sequences);

    ASSERT_TRUE(matrix.contains('a'));
    ASSERT_TRUE(matrix.contains('b'));

    double aa = matrix['a']['a'];
    double ab = matrix['a']['b'];
    double ac = matrix['a']['c'];
    EXPECT_NEAR(aa + ab + ac, 1.0, 1e-9);

    double ba = matrix['b']['a'];
    double bb = matrix['b']['b'];
    EXPECT_NEAR(ba + bb, 1.0, 1e-9);
}

TEST(GetTransitionsTest, ExistingAndMissingState) 
{
    auto matrix = generate_probability_matrix(char_sequences);

    const auto& transitions = get_transitions(matrix, 'a');
    EXPECT_GT(transitions.size(), 0);
    EXPECT_TRUE(transitions.contains('b'));

    const auto& missing = get_transitions(matrix, 'z');
    EXPECT_EQ(missing.size(), 0);
}

TEST(HasStateTest, DetectsPresence) 
{
    auto matrix = generate_probability_matrix(char_sequences);
    EXPECT_TRUE(has_state(matrix, 'a'));
    EXPECT_FALSE(has_state(matrix, 'z'));
}

TEST(GetStatesTest, ExtractsAllStates) 
{
    auto matrix = generate_probability_matrix(char_sequences);
    auto states = get_states(matrix);

    EXPECT_NE(std::find(states.begin(), states.end(), 'a'), states.end());
    EXPECT_NE(std::find(states.begin(), states.end(), 'b'), states.end());
}

TEST(ClearMatrixTest, EmptiesMatrix) 
{
    auto matrix = generate_probability_matrix(char_sequences);
    clear_matrix(matrix);
    EXPECT_TRUE(matrix.empty());
}

