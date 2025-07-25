/**
 * @file MarkovModels.hpp
 * @author Antonius Torode
 * @date 06/03/2025
 * Description: 
 *     This header defines generic functions for generating first-order
 *     and higher-order Markov models based on input sequences.
 *
 *     The resulting model is represented as a nested unordered map structure,
 *     where the outer key is the current state and the inner map holds
 *     probabilities of transitioning to possible next states.
 *
 *     This is useful for probabilistic sequence modeling, simulation, or generation,
 *     and can be applied to characters, words, states, or any other comparable type.
 *
 * Dependencies:
 *   - C++17 or later (for structured bindings and template aliasing)
 */
#pragma once

#include <unordered_map>
#include <vector>
#include <iostream>


namespace markov_models
{
    /**
     * Type alias for a probability transition matrix.
     * ProbabilityMatrix<T> maps a state of type T to a map of successor states and
     * their associated probabilities.
     */
    template <typename T>
    using ProbabilityMatrix = std::unordered_map<T, std::unordered_map<T, double>>;


    /**
     * Generates a probability transition matrix from a list of input sequences.
     * 
     * Each sequence is a vector of elements of type T. The function counts transitions
     * from each element to its successor in the sequence, and then normalizes the counts
     * to obtain transition probabilities.
     *
     * @tparam T - The type of elements in the sequences (e.g., char, std::string, etc.)
     * @param sequences[const std::vector<std::vector<T>>&] - A vector of sequences, where 
     *     each sequence is a vector of type T.
     * @return [ProbabilityMatrix] - A probability matrix representing transition probabilities 
     *     between states.
     */
    template <typename T>
    ProbabilityMatrix<T> generateProbabilityMatrix(const std::vector<std::vector<T>>& sequences) 
    {
        std::unordered_map<T, std::unordered_map<T, int>> countMatrix;

        // Count occurrences for each type in all sequences.
        for (const auto& seq : sequences) 
        {
            for (size_t i=0; i+1<seq.size(); ++i) 
            {
                countMatrix[seq[i]][seq[i + 1]]++;
            }
        }

        // Normalize occurrences to probabilities.
        ProbabilityMatrix<T> probMatrix;
        for (const auto& [input, outputs] : countMatrix) 
        {
            int total = 0;
            for (const auto& [_, count] : outputs) 
            {
                total += count;
            }
            for (const auto& [output, count] : outputs) 
            {
                probMatrix[input][output] = static_cast<double>(count) / total;
            }
        }

        return probMatrix;
    }

    /**
     * Retrieves the successor probabilities for a given input state.
     * @tparam T - The type of the state.
     * @param probMatrix [const ProbabilityMatrix<T>&] - The probability matrix generated 
     *     by generateProbabilityMatrix.
     * @param state [const T&] - The current state for which successors are requested.
     * @return A const reference to the unordered_map of successors and their probabilities.
     *         Returns an empty static map if the state is not found.
     */
    template <typename T>
    const std::unordered_map<T, double>& getTransitions(const ProbabilityMatrix<T>& probMatrix, 
                                                        const T& state) 
    {
        static const std::unordered_map<T, double> emptyMap{};
        auto it = probMatrix.find(state);
        if (it != probMatrix.end()) 
        {
            return it->second;
        }
        return emptyMap;
    }

    /**
     * Checks if a given state exists in the probability matrix.
     * @tparam T The type of the state.
     * @param matrix[const ProbabilityMatrix<T>&] - The probability matrix.
     * @param state[const T&] - The state to check.
     * @return [bool] - true if the state exists in the matrix, false otherwise.
     */
    template <typename T>
    bool hasState(const ProbabilityMatrix<T>& matrix, const T& state) 
    {
        return matrix.find(state) != matrix.end();
    }

    /**
     * Returns a vector containing all states present in the probability matrix.
     * @tparam T The type of the state.
     * @param matrix[const ProbabilityMatrix<T>&] - The probability matrix.
     * @return [std::vector<T>] - A vector of all states.
     */
    template <typename T>
    std::vector<T> getStates(const ProbabilityMatrix<T>& matrix) 
    {
        std::vector<T> states;
        states.reserve(matrix.size());
        for (const auto& [state, _] : matrix) 
        {
            states.push_back(state);
        }
        return states;
    }

    /**
     * Samples the next state from the probability distribution of a given state.
     * Uses a random number generator to select a successor state weighted by
     * the transition probabilities.
     * 
     * @tparam T The type of the state.
     * @param matrix[const ProbabilityMatrix<T>&] - The probability matrix.
     * @param state[const T&] - The current state from which to sample the next state.
     * @return The sampled next state.
     */
    template <typename T>
    T sampleNextState(const ProbabilityMatrix<T>& matrix, const T& state) 
    {
        // TODO - implement this.
    }

    /**
     * Prints the transitions for a specific state.
     * @tparam T The type of the state.
     * @param state[const T&] - The state whose transitions are to be printed.
     * @param transitions[const std::unordered_map<T, double>&] - Map of successor states and their probabilities.
     * @param os[std::ostream&] - The output stream to write to (defaults to std::cout).
     */
    template <typename T>
    void printTransition(const T& state, 
                         const std::unordered_map<T, double>& transitions, 
                         std::ostream& os = std::cout)
    {
        os << state << " -> { ";
        for (const auto& [next, prob] : transitions)
        {
            os << next << ": " << prob << ", ";
        }
        os << "}\n";
    }

    /**
     * Prints the entire probability matrix to the specified output stream.
     * Uses printTransition() to print each state's transitions.
     * @tparam T The type of the state.
     * @param matrix[const ProbabilityMatrix<T>&] - The probability matrix to print.
     * @param os[std::ostream&] - The output stream to write to (defaults to std::cout).
     */
    template <typename T>
    void printProbabilityMatrix(const ProbabilityMatrix<T>& matrix, 
                                std::ostream& os = std::cout)
    {
        for (const auto& [state, transitions] : matrix)
        {
            printTransition(state, transitions, os);
        }
    }

    /**
     * Clears all entries from the probability matrix.
     * @tparam T The type of the state.
     * @param matrix[ProbabilityMatrix<T>&] - The probability matrix to clear.
     */
    template <typename T>
    void clearMatrix(ProbabilityMatrix<T>& matrix) 
    {
        matrix.clear();
    }
} // namespace markov_models
