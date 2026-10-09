/**
 * @file Modifier.cpp
 * @author Antonius Torode
 * @date 10/08/2026
 * @brief Implements the modifier data and computation utilities declared in Modifier.hpp.
 */

// Include associated header file.
#include "Modifier.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <iomanip>
#include <limits>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

// Used for exception and error handling.
#include "MIAException.hpp"
#include "Error.hpp"

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


    Modifier::Modifier(uint32_t id, ModifierSourceType src, 
                       int val, 
                       ModifyType type,
                       ModifierStackPolicy policy) : 
        sourceID(id), 
        source(src), 
        value(val), 
        modifyType(type), 
        stackPolicy(policy) 
    {}


    Modifier::Modifier(uint32_t id, 
                       ModifierSourceType src, 
                       double val, 
                       ModifyType type,
                       ModifierStackPolicy policy) : 
        sourceID(id), 
        source(src), 
        value(val), 
        modifyType(type), 
        stackPolicy(policy) 
    {}


    Modifier::Modifier(uint32_t id, 
                       ModifierSourceType src, 
                       Value val, 
                       ModifyType type,
                       ModifierStackPolicy policy) : 
        sourceID(id), 
        source(src), 
        value(std::move(val)), 
        modifyType(type), 
        stackPolicy(policy)
    {}


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


    bool Modifier::isStrongerThan(const Modifier& other) const
    {
        if (modifyType != other.modifyType)
            return false;

        if (modifyType == ModifyType::MULTIPLY)
            return getValueAsDouble() > other.getValueAsDouble();

        return getValueAsInt() > other.getValueAsInt();
    }


    bool Modifier::operator==(const Modifier& other) const
    {
        return sourceID == other.sourceID && source == other.source &&
               modifyType == other.modifyType && stackPolicy == other.stackPolicy;
    }


    void attachModifier(std::vector<Modifier>& modifiers, const Modifier& modifier)
    {
        // The same source, source type, modify type, and stack policy is the same effect.
        auto exactMatch = std::find(modifiers.begin(), modifiers.end(), modifier);

        if (modifier.stackPolicy == ModifierStackPolicy::STACK)
        {
            // Stack modifiers accumulate, even re-applications from the same source.
            modifiers.push_back(modifier);
            return;
        }

        if (exactMatch != modifiers.end())
        {
            if (modifier.stackPolicy == ModifierStackPolicy::KEEP_STRONGEST &&
                !modifier.isStrongerThan(*exactMatch))
                return;

            *exactMatch = modifier;
            return;
        }

        if (modifier.stackPolicy != ModifierStackPolicy::KEEP_STRONGEST)
        {
            modifiers.push_back(modifier);
            return;
        }

        // A KEEP_STRONGEST modifier competes with every attached modifier of the same
        // source type and modify type, regardless of which source attached them.
        auto competing = std::find_if(modifiers.begin(), modifiers.end(),
            [&modifier](const Modifier& attached)
            {
                return attached.source == modifier.source &&
                       attached.modifyType == modifier.modifyType;
            });

        if (competing == modifiers.end())
        {
            modifiers.push_back(modifier);
            return;
        }

        if (modifier.isStrongerThan(*competing))
            *competing = modifier;
    } // void attachModifier()


    std::string Modifier::serialize() const
    {
        // max_digits10 guarantees the serialized double reads back as the same value.
        std::ostringstream valueStream;
        valueStream << std::setprecision(std::numeric_limits<double>::max_digits10);
        if (std::holds_alternative<int>(value))
            valueStream << std::get<int>(value);
        else
            valueStream << std::get<double>(value);

        return std::to_string(sourceID) + ":" +
               modifierSourceTypeToString(source) + ":" +
               valueStream.str() + ":" +
               modifyTypeToString(modifyType) + ":" +
               modifierStackPolicyToString(stackPolicy);
    }


    Modifier Modifier::deserialize(const std::string& data)
    {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(data);

        while (std::getline(tokenStream, token, ':'))
            tokens.push_back(token);

        if (tokens.size() != 5)
            MIA_THROW(error::Invalid_RPG_Data, "Invalid Modifier data format");

        uint32_t sourceID = std::stoul(tokens[0]);
        ModifierSourceType source = stringToModifierSourceType(tokens[1]);
        double parsedValue = std::stod(tokens[2]);
        ModifyType type = stringToModifyType(tokens[3]);
        ModifierStackPolicy policy = stringToModifierStackPolicy(tokens[4]);

        // Store the value as an int for ADD_MAX and SET, and as a double for MULTIPLY.
        Value value = (type == ModifyType::MULTIPLY)
                          ? Value(parsedValue)
                          : Value(static_cast<int>(std::round(parsedValue)));

        return Modifier(sourceID, source, value, type, policy);
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
        os << ", type=" << modifyTypeToString(modifier.modifyType)
           << ", policy=" << modifierStackPolicyToString(modifier.stackPolicy) << "}";
        return os;
    }
} // namespace rpg
