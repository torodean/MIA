/**
 * @file ProgressMarker.hpp
 * @author Antonius Torode
 * @date 07/06/2025
 * Description: This is the base progression class for all progress markers.
 */
#pragma once

#include <stdint.h>
#include <string>
#include <nlohmann/json.hpp>

#include "BaseDataObject.hpp"

namespace progress
{
    /**
     * This is a base class for all progression markers. It contains common information which
     * all progression markers use (or should use).
     */
    class ProgressMarker : public rpg::BaseDataObject
    {
    public:
        /// Default constructor.
        ProgressMarker() = default;
        
        /**
         * The main constructor for a ProgressMarker object.
         * @param name[const std::string&] - The name of the progress.
         * @param description[const std::string&] - The desciption of the progress.
         */
        ProgressMarker(uint32_t id,
                       const std::string& name, 
                       const std::string& description);
        
        /**
         * Serializes the ProgressMarker object into a JSON representation.
         *
         * @return A nlohmann::json object containing the progress's ID, name,
         *         and description.
         */
        nlohmann::json toJson() const override;
        
        /**
         * Deserializes a ProgressMarker object from JSON.
         *
         * @param json The JSON object containing ProgressMarker properties.
         * @return The constructed ProgressMarker object.
         */
        static ProgressMarker fromJson(const nlohmann::json& json);
        
    }; // class ProgressMarker
} // namespace progress
