/**
 * File: CharacterVitals.cpp
 * Author: Antonius Torode
 * Created on: 07/07/2025
 * Description: A class representing the vitals for a character.
 */

#include <iostream>
#include <sstream>
#include "CharacterVitals.hpp"

namespace rpg
{
    std::string CharacterVitals::serialize() const
    {
        std::ostringstream oss;
        oss << "[CV_BEGIN]";
        
        auto writeVital = [&](const Vital& v) 
        {
            oss << v.current << ',' << v.max << ',' << v.min << ',';
        };

        writeVital(health);
        writeVital(mana);
        writeVital(energy);
        writeVital(rage);
        writeVital(breath);
        writeVital(focus);
        writeVital(radiation);
        writeVital(temperature);
        writeVital(hunger);
        writeVital(thirst);
        writeVital(toxicity);
        writeVital(sanity);

        std::string result = oss.str();
        if (!result.empty()) result.pop_back(); // Remove trailing comma
        result += "[CV_END]";
        return result;
    }


    bool CharacterVitals::deserialize(const std::string& data)
    {
        const std::string startTag = "[CV_BEGIN]";
        const std::string endTag = "[CV_END]";
        auto startPos = data.find(startTag);
        if (startPos == std::string::npos) return false;
        startPos += startTag.length();

        auto endPos = data.find(endTag, startPos);
        if (endPos == std::string::npos) return false;

        std::string payload = data.substr(startPos, endPos - startPos);
        std::istringstream iss(payload);

        constexpr int kVitalFieldCount = 3;
        constexpr int kVitalCount = 12;
        int values[kVitalFieldCount * kVitalCount];
        char comma;

        for (int i = 0; i < 36; ++i)
        {
            if (!(iss >> values[i]))
                return false;
            if (i < 35 && !(iss >> comma && comma == ','))
                return false;
        }

        int idx = 0;
        
        auto readVital = [&](Vital& v) 
        {
            v.current = values[idx++];
            v.max = values[idx++];
            v.min = values[idx++];
        };

        readVital(health);
        readVital(mana);
        readVital(energy);
        readVital(rage);
        readVital(breath);
        readVital(focus);
        readVital(radiation);
        readVital(temperature);
        readVital(hunger);
        readVital(thirst);
        readVital(toxicity);
        readVital(sanity);

        return true;
    }

} // namespace rpg
