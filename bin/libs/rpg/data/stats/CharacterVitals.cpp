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
    Vital CharacterVitals::getVital(const std::string& name) const
    {
        auto it = vitals.find(name);
        return it != vitals.end() ? it->second : Vital{};
    }


    void CharacterVitals::setVital(const std::string& name, const Vital& vital)
    {
        Vital& member = vitals[name];
        member.current = std::clamp(vital.current, vital.min, vital.max);
        member.max = vital.max;
        member.min = vital.min;
    }


    bool CharacterVitals::addVital(const std::string& name)
    {
        if (vitals.find(name) != vitals.end())
            return false;
        vitals[name] = Vital{};
        return true;
    }


    bool CharacterVitals::removeVital(const std::string& name)
    {
        return vitals.erase(name) > 0;
    }


    std::string CharacterVitals::serialize() const
    {
        std::ostringstream oss;
        oss << "[CV_BEGIN]";
        bool first = true;
        for (const auto& [name, vital] : vitals)
        {
            if (!first)
                oss << ";";
            oss << name << ":" << vital.current << "," << vital.max << "," << vital.min;
            first = false;
        }
        oss << "[CV_END]";
        return oss.str();
    }


    bool CharacterVitals::deserialize(const std::string& data)
    {
        const std::string beginMarker = "[CV_BEGIN]";
        const std::string endMarker = "[CV_END]";

        // Find the vitals block
        size_t start = data.find(beginMarker);
        if (start == std::string::npos)
            return false;
        start += beginMarker.length();
        size_t end = data.find(endMarker, start);
        if (end == std::string::npos)
            return false;

        // Clear existing vitals
        vitals.clear();

        // Extract and parse the vitals string
        std::string vitalsData = data.substr(start, end - start);
        if (vitalsData.empty())
            return true;

        std::istringstream iss(vitalsData);
        std::string vitalEntry;
        while (std::getline(iss, vitalEntry, ';'))
        {
            // Parse each vital: name:current,max,min
            size_t colonPos = vitalEntry.find(':');
            if (colonPos == std::string::npos)
                return false;
            std::string name = vitalEntry.substr(0, colonPos);
            std::string values = vitalEntry.substr(colonPos + 1);

            std::istringstream valueStream(values);
            std::string currentStr, maxStr, minStr;
            if (!std::getline(valueStream, currentStr, ',') ||
                !std::getline(valueStream, maxStr, ',') ||
                !std::getline(valueStream, minStr))
                return false;

            try
            {
                Vital vital;
                vital.current = std::stoi(currentStr);
                vital.max = std::stoi(maxStr);
                vital.min = std::stoi(minStr);
                vitals[name] = vital;
            }
            catch (...)
            {
                return false;
            }
        }
        return true;
    }

} // namespace rpg
