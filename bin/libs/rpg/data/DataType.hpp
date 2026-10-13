/**
 * @file DataType.hpp
 * @author Antonius Torode
 * @date 07/06/2025
 * @brief A class to store all the valid/possible data types.
 */
#pragma once

#include <string>

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
		PROGRESS,   ///< Represents a progress marker object.
	};
	
	/**
	 * @brief Converts a DataType enum value to its string representation.
	 * 
	 * @param type The DataType enum value.
	 * @return std::string The string name of the enum value.
	 */
	std::string dataTypeToString(DataType type);
	
	/**
	 * @brief Converts a string to the corresponding DataType enum value.
	 * 
	 * @param str The input string representing a DataType name.
	 * @return DataType The corresponding enum value if matched, otherwise DataType::UNKNOWN.
	 */
	DataType stringToDataType(const std::string& str);
} // namespace rpg
