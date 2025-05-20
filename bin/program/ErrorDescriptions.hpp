/**
 * @file ErrorDescriptions.hpp
 * @author Antonius Torode
 * @date 5/20/2025
 * @brief Declares access to human-readable descriptions of MIA error codes.
 */
#pragma once

#include <string>
#include <unordered_map>

// For ErrorCode type.
#include "Error.hpp"

namespace error
{
    /**
     * @brief Retrieves the human-readable description for a given ErrorCode.
     * @param code The ErrorCode to look up.
     * @return A const reference to the corresponding error description string.
     */
    const std::string& getErrorDescription(ErrorCode code);
}
