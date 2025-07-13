/**
 * @file CharacterMarkovModel.cpp
 * @author Antonius Torode
 * @date 06/05/2025
 * Description:
 *     This file implementats the CharacterMarkovModel.
 */

#include <string>
#include <vector>
#include "CharacterMarkovModel.hpp"
#include "StringUtils.hpp"

namespace markov_models
{
    CharacterMarkovModel::CharacterMarkovModel(const std::vector<std::string>& sequences)
    {
        trainFromStrings(sequences);
    }
    
    
    void CharacterMarkovModel::trainFromStrings(const std::vector<std::string>& sequences)    
    {
        std::vector<std::vector<char>> trainingVec;
        for (const auto& sequence : sequences)
        {
            trainingVec.push_back( StringUtils::stringToCharVec(sequence) );
        }
        train(trainingVec);
    }
} // namespace markov_models
