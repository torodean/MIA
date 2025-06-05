/*
 * File: AbstractMarkovModel.hpp
 * Author: Antonius Torode
 * Created on: 06/05/2025
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
        virtual void train(const std::vector<std::vector<T>>& sequences) = 0;

        /**
         * Samples the next state based on the current state.
         * @param currentState[const T&] - The current state.
         * @return [T] - The next state sampled from the learned distribution.
         */
        virtual T sampleNextState(const T& currentState) const = 0;

        /**
         * Returns a reference to the internal probability matrix.
         * @return [const ProbabilityMatrix<T>&] - The probability transition matrix.
         */
        virtual const ProbabilityMatrix<T>& getMatrix() const = 0;

        /**
         * Checks if the given state exists in the model.
         * @param state[const T&] - The state to check.
         * @return [bool] - True if the state exists, false otherwise.
         */
        virtual bool hasState(const T& state) const = 0;

        /**
         * Retrieves the set of successor states and their probabilities for a given state.
         * @param state[const T&] - The current state.
         * @return [const std::unordered_map<T, double>& - A const reference to the map of successor states.
         */
        virtual const std::unordered_map<T, double>& getTransition(const T& state) const = 0;

        /**
         * Returns a list of all states in the model.
         * @return [std::vector<T>] - A vector of states.
         */
        virtual std::vector<T> getAllStates() const = 0;

        /**
         * Clears the contents of the model.
         */
        virtual void clear() = 0;

        /**
         * Prints the model's successor probabilities for a specific state to the provided output stream.
         * @param state[const T&] - The state to print transitions for.
         * @param os The output stream to print to.
         */
        virtual void printTransition(const T& state, std::ostream& os = std::cout) const = 0;        
        
        /**
         * Prints the model's transition matrix to the provided output stream.
         * @param os The output stream to print to.
         */
        virtual void print(std::ostream& os = std::cout) const = 0;
    };
} // namespace markov_models

