/**
 * @file CharUtils.cpp
 * @author Antonius Torode
 * @date 09/17/2026
 * @brief Implements the methods for CharUtils.
 */

#include <cctype>

namespace char_utils
{
	char toUpper(char in)
	{
		return static_cast<char>(std::toupper(static_cast<unsigned char>(in)));
	}
} // namespace char_utils