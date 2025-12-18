/**
 * @file ProgressMarker.hpp
 * @author Antonius Torode
 * @date 07/06/2025
 * Description: This is the base progression class for all progress markers.
 */

#include "ProgressMarker.hpp"

namespace progress
{

    ProgressMarker::ProgressMarker(uint32_t id,
                                   const std::string& name, 
                                   const std::string& description) : 
        BaseDataObject(id, name, description)
    { }
    

    nlohmann::json ProgressMarker::toJson() const
    {
        nlohmann::json json = BaseDataObject::toJson();
        return json;
    }
    

    ProgressMarker ProgressMarker::fromJson(const nlohmann::json& json)
    {
        BaseDataObject base = BaseDataObject::fromJson(json);

        ProgressMarker progressMarker(base.getID(), base.getName(), base.getDescription());
        return progressMarker;
    }

} // namespace progress
