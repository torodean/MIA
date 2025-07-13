/**
 * @file DataType.hpp
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
	 * @enum DataType
	 * @brief Represents the types of data objects in the RPG system.
	 */
	enum class DataType
	{
		UNKNOWN,    ///< Represents an unknown data object.
		CURRENCY,   ///< Represents a Currency object.
		VITAL,      ///< Represents a Vital object.
		ATTRIBUTE,  ///< Represnts an Attribute object.
	};
	
	/**
	 * @brief Converts a DataType enum value to its string representation.
	 * 
	 * @param type The DataType enum value.
	 * @return std::string The string name of the enum value.
	 */
	inline std::string dataTypeToString(DataType type)
	{
		switch (type)
		{
			case DataType::UNKNOWN:   return "UNKNOWN";
			case DataType::CURRENCY:  return "CURRENCY";
			case DataType::VITAL:     return "VITAL";
			case DataType::ATTRIBUTE: return "ATTRIBUTE";
			default:                   return "UNKNOWN";
		}
	}
	
	/**
	 * @brief Converts a string to the corresponding DataType enum value.
	 * 
	 * @param str The input string representing a DataType name.
	 * @return DataType The corresponding enum value if matched, otherwise DataType::UNKNOWN.
	 */
	inline DataType stringToDataType(const std::string& str)
	{
		if (str == "UNKNOWN")   return DataType::UNKNOWN;
		if (str == "CURRENCY")  return DataType::CURRENCY;
		if (str == "VITAL")     return DataType::VITAL;
		if (str == "ATTRIBUTE") return DataType::ATTRIBUTE;
		return DataType::UNKNOWN;
	}
} // namespace rpg
