/**
 * @file CharacterMarkovModel.hpp
 * @author Antonius Torode
 * @date 06/05/2025
 * Description:
 *     This header defines an implementation of the AbstractMarkovModel designed
 *     to take sequences of characters (input as words) as each set. A set of
 *     words is then used to defined the set of sequences.
 */
#pragma once

#include <vector>
#include "AbstractMarkovModel.hpp"

namespace markov_models
{
    /**
     * This class implements a first order markov model which uses
     * characters as the element type. The class is setup such that
     * strings can be input as the sequence rather than a vector of
     * characters.
     */
    class CharacterMarkovModel : public AbstractMarkovModel<char>
    {
    public:
        /**
         * Default constructor. This will initialize with an empty probabilityMatrix.
         * If used, the trainFromStrings() method should be called to train the data. 
         */
        CharacterMarkovModel() = default;
        
        /**
         * The main constructor for this object. This will call the appropriate
         * train() method to generate and fill the probabilityMatrix.
         * @param sequences[const std::vector<std::string>&] - The list of sequences
         *     to use for training and generating the probabilityMatrix.
         */
        CharacterMarkovModel(const std::vector<std::string>& sequences);
        
        /// Default destructor.
        ~CharacterMarkovModel() = default;
        
        /**
         * This method will convert an input vector of sequences into a
         * std::vector<std::vector<char>>, then call the train() method in the base
         * class using that input vector to generate the probabilityMatrix.
         * @param sequences[const std::vector<std::string>&] - The list of sequences
         *     to use for training and generating the probabilityMatrix.
         */
        void trainFromStrings(const std::vector<std::string>& sequences);
        
    }; // class CharacterMarkovModel
} // namespace markov_models
