/**
 * @file DataType.cpp
 * @author Antonius Torode
 * @date 10/12/2026
 * @brief Implements a class to store all the valid/possible data types.
 */

// Include the associated header file.
#include "DataType.hpp"

#include <iostream>

namespace rpg
{
	std::string dataTypeToString(DataType type)
	{
		switch (type)
		{
			case DataType::UNKNOWN:   return "UNKNOWN";
			case DataType::CURRENCY:  return "CURRENCY";
			case DataType::VITAL:     return "VITAL";
			case DataType::ATTRIBUTE: return "ATTRIBUTE";
			case DataType::PROGRESS:  return "PROGRESS";
			default:                  return "UNKNOWN";
		}
	}


	DataType stringToDataType(const std::string& str)
	{
		if (str == "UNKNOWN")   return DataType::UNKNOWN;
		if (str == "CURRENCY")  return DataType::CURRENCY;
		if (str == "VITAL")     return DataType::VITAL;
		if (str == "ATTRIBUTE") return DataType::ATTRIBUTE;
		if (str == "PROGRESS")  return DataType::PROGRESS;
		return DataType::UNKNOWN;
	}
} // namespace rpg
