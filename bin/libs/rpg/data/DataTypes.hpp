/**
 * @file DataTypes.hpp
 * @author Antonius Torode
 * @date 07/06/2025
 * Description: A class to store all the valid/possible data types.
 */
#pragma once

#include <string>
#include <iostream>

namespace rpg
{
	/**
	 * @enum DataTypes
	 * @brief Represents the types of data objects in the RPG system.
	 */
	enum class DataTypes
	{
		UNKNOWN,    ///< Represents an unknown data object.
		CURRENCY,   ///< Represents a Currency object.
		VITAL,      ///< Represents a Vital object.
		ATTRIBUTE,  ///< Represnts an Attribute object.
	}
	
	/**
	 * @brief Converts a DataTypes enum value to its string representation.
	 * 
	 * @param type The DataTypes enum value.
	 * @return std::string The string name of the enum value.
	 */
	inline std::string dataTypeToString(DataTypes type)
	{
		switch (type)
		{
			case DataTypes::UNKNOWN:   return "UNKNOWN";
			case DataTypes::CURRENCY:  return "CURRENCY";
			case DataTypes::VITAL:     return "VITAL";
			case DataTypes::ATTRIBUTE: return "ATTRIBUTE";
			default:                   return "UNKNOWN";
		}
	}
	
	/**
	 * @brief Converts a string to the corresponding DataTypes enum value.
	 * 
	 * @param str The input string representing a DataTypes name.
	 * @return DataTypes The corresponding enum value if matched, otherwise DataTypes::UNKNOWN.
	 */
	inline DataTypes stringToDataType(const std::string& str)
	{
		if (str == "UNKNOWN")   return DataTypes::UNKNOWN;
		if (str == "CURRENCY")  return DataTypes::CURRENCY;
		if (str == "VITAL")     return DataTypes::VITAL;
		if (str == "ATTRIBUTE") return DataTypes::ATTRIBUTE;
		return DataTypes::UNKNOWN;
	}
} // namespace rpg
