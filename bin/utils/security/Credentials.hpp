/**
 * @file Credentials.hpp
 * @author Antonius Torode
 * @date 06/30/25
 * Description: This is a file for storing and dealing with credentials for a user.
 */
#pragma once

#include <string>

namespace credentials
{
	/**
	 * Prompts the user to enter a password with masked input.
	 *
	 * The function reads input character-by-character from the terminal,
	 * displays a '*' for each printable character typed, and supports backspace.
	 * It returns the entered password as a std::string.
	 *
	 * Platform-specific behavior:
	 * - On Windows, uses _getch() for raw character input.
	 * - On POSIX systems (Linux/macOS), disables terminal echo and canonical mode.
	 *
	 * @return std::string The password entered by the user.
	 */
	std::string password();
	
} // namespace credentials