/*
 * File: MarkovModels.hpp
 * Author: Antonius Torode
 * Created on: 06/03/2025
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
    ProbabilityMatrix<T> generate_probability_matrix(const std::vector<std::vector<T>>& sequences) 
    {
        std::unordered_map<T, std::unordered_map<T, int>> count_matrix;

        // Count occurrences for each type in all sequences.
        for (const auto& seq : sequences) 
        {
            for (size_t i=0; i+1<seq.size(); ++i) 
            {
                count_matrix[seq[i]][seq[i + 1]]++;
            }
        }

        // Normalize occurrences to probabilities.
        ProbabilityMatrix<T> prob_matrix;
        for (const auto& [input, outputs] : count_matrix) 
        {
            int total = 0;
            for (const auto& [_, count] : outputs) 
            {
                total += count;
            }
            for (const auto& [output, count] : outputs) 
            {
                prob_matrix[input][output] = static_cast<double>(count) / total;
            }
        }

        return prob_matrix;
    }
} // namespace markov_models
