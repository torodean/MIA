/**
 * @file Modifier.cpp
 * @author Antonius Torode
 * @date 10/08/2026
 * @brief Implements the modifier data and computation utilities declared in Modifier.hpp.
 */

#include "Modifier.hpp"

// Used for exception and error handling.
#include "MIAException.hpp"
#include "Error.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <limits>
#include <ostream>
#include <string>
#include <utility>

namespace rpg
{
    std::string modifierSourceTypeToString(const ModifierSourceType& type)
    {
        switch (type)
        {
            case ModifierSourceType::ATTRIBUTE: return "ATTRIBUTE";
            case ModifierSourceType::ITEM:      return "ITEM";
            case ModifierSourceType::BUFF:      return "BUFF";
            case ModifierSourceType::DEBUFF:    return "DEBUFF";
            default:                            return "UNKNOWN";
        }
    }

    ModifierSourceType stringToModifierSourceType(const std::string& typeStr)
    {
        std::string str = typeStr;
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);

        if (str == "ATTRIBUTE") return ModifierSourceType::ATTRIBUTE;
        if (str == "ITEM")      return ModifierSourceType::ITEM;
        if (str == "BUFF")      return ModifierSourceType::BUFF;
        if (str == "DEBUFF")    return ModifierSourceType::DEBUFF;
        return ModifierSourceType::UNKNOWN;
    }

    Modifier::Modifier(uint32_t id, ModifierSourceType src, int val, ModifyType type)
        : sourceID(id), source(src), value(val), modifyType(type) {}

    Modifier::Modifier(uint32_t id, ModifierSourceType src, double val, ModifyType type)
        : sourceID(id), source(src), value(val), modifyType(type) {}

    Modifier::Modifier(uint32_t id, ModifierSourceType src, Value val, ModifyType type)
        : sourceID(id), source(src), value(std::move(val)), modifyType(type) {}

    int Modifier::getValueAsInt() const
    {
        if (!std::holds_alternative<int>(value))
            MIA_THROW(error::Invalid_RPG_Data,
                      "Modifier value is not an int for modify type " +
                      modifyTypeToString(modifyType) + ".");
        return std::get<int>(value);
    }

    double Modifier::getValueAsDouble() const
    {
        if (!std::holds_alternative<double>(value))
            MIA_THROW(error::Invalid_RPG_Data,
                      "Modifier value is not a double for modify type " +
                      modifyTypeToString(modifyType) + ".");
        return std::get<double>(value);
    }

    bool Modifier::operator==(const Modifier& other) const
    {
        return sourceID == other.sourceID && source == other.source &&
               modifyType == other.modifyType;
    }

    int computeModifiedValue(int base, const std::vector<Modifier>& modifiers)
    {
        double value = static_cast<double>(base);

        // Phase 1: multipliers scale the base.
        for (const auto& mod : modifiers)
        {
            if (mod.modifyType == ModifyType::MULTIPLY)
                value *= (1.0 + mod.getValueAsDouble());
        }

        // Phase 2: additive modifiers sum onto the multiplied value.
        for (const auto& mod : modifiers)
        {
            if (mod.modifyType == ModifyType::ADD_MAX)
                value += mod.getValueAsInt();
        }

        // Phase 3: a set override replaces the value; the last attached one wins.
        for (const auto& mod : modifiers)
        {
            if (mod.modifyType == ModifyType::SET)
                value = mod.getValueAsInt();
        }

        double clamped = std::clamp(value,
                                    static_cast<double>(std::numeric_limits<int>::min()),
                                    static_cast<double>(std::numeric_limits<int>::max()));
        return static_cast<int>(std::round(clamped));
    }

    std::ostream& operator<<(std::ostream& os, const Modifier& modifier)
    {
        os << "Modifier{sourceID=" << modifier.sourceID
           << ", source=" << modifierSourceTypeToString(modifier.source)
           << ", value=";
        if (std::holds_alternative<int>(modifier.value))
            os << std::get<int>(modifier.value);
        else
            os << std::get<double>(modifier.value);
        os << ", type=" << modifyTypeToString(modifier.modifyType) << "}";
        return os;
    }
} // namespace rpg
