/**
 * @file AbstractMarkovModel.hpp
 * @author Antonius Torode
 * @date 06/05/2025
 * Description:
 *     This header defines an abstract base class template for Markov models.
 *     It provides a common interface for derived classes to implement first-order
 *     Markov models over arbitrary state types.
 *
 *     Implementations should provide their own logic for training, sampling,
 *     and accessing model internals. This abstraction enables interchangeable
 *     use of different model strategies in generic pipelines.
 *
 * Dependencies:
 *   - C++17 or later (for template aliasing and override syntax)
 */
#pragma once

#include "MarkovModels.hpp"

namespace markov_models
{
    /**
     * Abstract interface for a generic first-order Markov model.
     * @tparam T The type of the state (e.g., char, std::string, custom struct, etc).
     */
    template <typename T>
    class AbstractMarkovModel
    {
    public:
    
        virtual ~AbstractMarkovModel() = default;
    
        /**
         * Trains the model using the given sequences of states.
         * @param sequences[const std::vector<std::vector<T>>&] - A vector of input sequences.
         */
        inline void train(const std::vector<std::vector<T>>& sequences)
        { probabilityMatrix = markov_models::generateProbabilityMatrix(sequences); };

        /**
         * Checks if the given state exists in the model.
         * @param state[const T&] - The state to check.
         * @return [bool] - True if the state exists, false otherwise.
         */
        inline bool hasState(const T& state) const
        { return markov_models::hasState(probabilityMatrix, state); };

        /**
         * Retrieves the set of successor states and their probabilities for a given state.
         * @param state[const T&] - The current state.
         * @return [const std::unordered_map<T, double>& - A const reference to the map of successor states.
         */
        inline const std::unordered_map<T, double>& getTransition(const T& state) const
        { return markov_models::getTransitions(probabilityMatrix, state); };

        /**
         * Returns a list of all states in the model.
         * @return [std::vector<T>] - A vector of states.
         */
        inline std::vector<T> getAllStates() const
        { return markov_models::getStates(probabilityMatrix); };
        
        /**
         * Prints the model's probability matrix to the provided output stream.
         * @param os The output stream to print to.
         */
        inline void print(std::ostream& os = std::cout) const
        { markov_models::printProbabilityMatrix(probabilityMatrix); };

        /**
         * Clears the contents of the model.
         */
        inline void clear()
        { markov_models::clearMatrix(probabilityMatrix); };

        /**
         * Returns a reference to the internal probability matrix.
         * @return [const ProbabilityMatrix<T>&] - The probability transition matrix.
         */
        inline const ProbabilityMatrix<T>& getMatrix() const
        { return probabilityMatrix; };

        /**
         * Prints the model's successor probabilities for a specific state to the provided output stream.
         * @param state[const T&] - The state to print transitions for.
         * @param os The output stream to print to.
         */
        inline void printTransition(const T& state, std::ostream& os = std::cout) const
        {
            auto it = probabilityMatrix.find(state);
            if (it != probabilityMatrix.end())
                printTransition(state, it->second, os);
            else
                os << "State " << state << " not found in model." << std::endl;
        };

        /**
         * Samples the next state based on the current state.
         * @param state[const T&] - The current state.
         * @return [T] - The next state sampled from the learned distribution.
         */
        inline T sampleNextState(const T& state) const
        { return markov_models::sampleNextState(probabilityMatrix, state); };
        
    private:
        
        /// Storage for the main probability matrix of this class.
        ProbabilityMatrix<T> probabilityMatrix;
    };
} // namespace markov_models

