/**
 * File: Vitals.cpp
 * Author: Antonius Torode
 * Created on: 07/07/2025
 * Description: A class representing the vitals for a character.
 */

#include <iostream>
#include <sstream>
#include "Vitals.hpp"

namespace rpg
{
    Vital Vitals::getVital(const std::string& name) const
    {
        auto itId = nameToId.find(name);
        if (itId == nameToId.end())
            return Vital{};  // Not found, return default Vital
        auto itVital = vitals.find(itId->second);
        return itVital != vitals.end() ? itVital->second : Vital{};
    }

    Vital Vitals::getVital(uint32_t id) const
    {
        auto it = vitals.find(id);
        return it != vitals.end() ? it->second : Vital{};
    }

    Vital Vitals::getVital(const Vital& vital) const
    {
        // Match by id if set, otherwise by all fields
        if (vital.getID() != 0)
        {
            return getVital(vital.getID());
        }

        // Otherwise, try to find by matching all vital fields.
        for (const auto& [id, storedVital] : vitals)
        {
            if (storedVital.getName() == vital.getName() &&
                storedVital.getDescription() == vital.getDescription() &&
                storedVital.getVitalType() == vital.getVitalType() &&
                storedVital.getCurrent() == vital.getCurrent() &&
                storedVital.getMax() == vital.getMax() &&
                storedVital.getMin() == vital.getMin())
            {
                return storedVital;
            }
        }

        return Vital{};
    }
    
    bool Vitals::updateVitalCurrent(const std::string& name, int current)
    {
        auto itId = nameToId.find(name);
        if (itId == nameToId.end()) return false;
        auto it = vitals.find(itId->second);
        if (it == vitals.end()) return false;
        it->second.setCurrent(current);
        return true;
    }

    bool Vitals::updateVitalCurrent(uint32_t id, int current)
    {
        auto it = vitals.find(id);
        if (it == vitals.end()) return false;
        it->second.setCurrent(current);
        return true;
    }

    // Similarly for updateVitalMin and updateVitalMax:
    bool Vitals::updateVitalMin(const std::string& name, int min)
    {
        auto itId = nameToId.find(name);
        if (itId == nameToId.end()) return false;
        auto it = vitals.find(itId->second);
        if (it == vitals.end()) return false;
        it->second.setMin(min);
        return true;
    }

    bool Vitals::updateVitalMin(uint32_t id, int min)
    {
        auto it = vitals.find(id);
        if (it == vitals.end()) return false;
        it->second.setMin(min);
        return true;
    }

    bool Vitals::updateVitalMax(const std::string& name, int max)
    {
        auto itId = nameToId.find(name);
        if (itId == nameToId.end()) return false;
        auto it = vitals.find(itId->second);
        if (it == vitals.end()) return false;
        it->second.setMax(max);
        return true;
    }

    bool Vitals::updateVitalMax(uint32_t id, int max)
    {
        auto it = vitals.find(id);
        if (it == vitals.end()) return false;
        it->second.setMax(max);
        return true;
    }

    bool Vitals::addVital(const std::string& name)
    {
        // TODO
    }

    bool Vitals::addVital(uint32_t id)
    {
        // TODO
    }

    bool Vitals::addVital(const Vital& vital)
    {
        // TODO
    }

    bool Vitals::removeVital(const std::string& name)
    {
        auto itId = nameToId.find(name);
        if (itId == nameToId.end())
            return false;
        uint32_t id = itId->second;
        nameToId.erase(itId);
        return vitals.erase(id) > 0;
    }

    bool Vitals::removeVital(uint32_t id)
    {
        // Remove name mapping if exists
        auto itName = std::find_if(nameToId.begin(), nameToId.end(),
            [id](const auto& pair) { return pair.second == id; });
        if (itName != nameToId.end())
            nameToId.erase(itName);
        return vitals.erase(id) > 0;
    }

    bool Vitals::removeVital(const Vital& vital)
    {
        for (auto it = vitals.begin(); it != vitals.end(); ++it)
        {
            if (it->second.getCurrent() == vital.getCurrent() &&
                it->second.getMin() == vital.getMin() &&
                it->second.getMax() == vital.getMax())
            {
                // Erase associated name
                auto itName = std::find_if(nameToId.begin(), nameToId.end(),
                    [&](const auto& pair) { return pair.second == it->first; });
                if (itName != nameToId.end())
                    nameToId.erase(itName);

                vitals.erase(it);
                return true;
            }
        }
        return false;
    }


    std::string Vitals::serialize() const
    {
        std::ostringstream oss;
        oss << "[CV_BEGIN]";
        bool first = true;
        for (const auto& [id, vital] : vitals)
        {
            if (!first)
                oss << ";";
            oss << id << ":" << vital.getCurrent() << "," << vital.getMax() << "," << vital.getMin();
            first = false;
        }
        oss << "[CV_END]";
        return oss.str();
    }


    bool Vitals::deserialize(const std::string& data)
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
            // Parse each vital: id:current,max,min
            size_t colonPos = vitalEntry.find(':');
            if (colonPos == std::string::npos)
                return false;
            std::string id = vitalEntry.substr(0, colonPos);
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
                vital.setCurrent( std::stoi(currentStr) );
                vital.setMax( std::stoi(maxStr) );
                vital.setMin( std::stoi(minStr) );
                vitals[static_cast<uint32_t>(std::stoul(id))] = vital;
            }
            catch (...)
            {
                return false;
            }
        }
        return true;
    }

} // namespace rpg
