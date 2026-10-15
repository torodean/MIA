/**
 * @file BaseDataObject.cpp
 * @author Antonius Torode
 * @date 10/12/2026
 * @brief Implements the base data object class for rpg data classes.
 */

// Include the associated header file.
#include "BaseDataObject.hpp"

namespace rpg
{
    BaseDataObject::BaseDataObject(uint32_t id,
                                   const std::string& name,
                                   const std::string& description) :
        id(id),
        name(name),
        description(description)
    { }


    uint32_t BaseDataObject::getID() const 
    { 
        return id; 
    }


    std::string BaseDataObject::getName() const 
    { 
        return name; 
    }


    std::string BaseDataObject::getDescription() const 
    { 
        return description; 
    }


    nlohmann::json BaseDataObject::toJson() const
    {
        return
        {
            {"id", id},
            {"name", name},
            {"description", description}
        };
    }


    BaseDataObject BaseDataObject::fromJson(const nlohmann::json& json)
    {
        auto id = json.at("id").get<uint32_t>(); // Use at() to require id.
        auto name = json.at("name").get<std::string>();
        auto description = json.value("description", "");

        BaseDataObject baseDataObj(id, name, description);

        return baseDataObj;
    }
} // namespace rpg

