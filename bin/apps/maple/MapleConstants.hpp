/**
 * @file MapleConstants.hpp
 * @author Antonius Torode
 * @date 08/19/2026
 * @brief Defines constants shared across the Maple app.
 */
#pragma once

#include <string>

namespace maple
{
    /**
     * @brief The scope value for an item that applies in every scenario.
     *
     * Used by Income and MonthlyExpenses for sources and expenses that are not
     * tied to a specific scenario.
     */
    inline const std::string ALL_SCOPE{"all"};
} // namespace maple
