/**
 * @file Credentials.cpp
 * @author Antonius Torode
 * @date 06/30/25
 * Description: Implementation file for Credentials.hpp.
 */

#include <iostream>
#include <string>

#include <termios.h>
#include <unistd.h>

#include "Credentials.hpp"
#include "Constants.hpp"

namespace credentials
{
	std::string password() 
	{
		std::string pwd;
		std::cout << "Enter password: " << std::flush;

		termios oldt, newt;
		tcgetattr(STDIN_FILENO, &oldt);           // get current terminal settings
		newt = oldt;
		newt.c_lflag &= ~(ECHO | ICANON);         // disable echo and canonical mode
		tcsetattr(STDIN_FILENO, TCSANOW, &newt);  // apply new settings

		char ch;
		while (read(STDIN_FILENO, &ch, 1) == 1 && ch != '\n') 
		{
			if (ch == 127 || ch == '\b') 
			{ // Backspace
				if (!pwd.empty()) 
				{
					pwd.pop_back();
					std::cout << "\b \b" << std::flush;
				}
			} 
			else if (isprint(ch)) 
			{
				pwd.push_back(ch);
				std::cout << '*' << std::flush;
			}
		}

		tcsetattr(STDIN_FILENO, TCSANOW, &oldt);  // restore old settings

		std::cout << std::endl;
		return pwd;
	}
} // namespace credentials