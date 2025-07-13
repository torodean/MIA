/**
 * @file WindowsUtils.cpp
 * @author Antonius Torode
 * @date 03/07/2021
 * Description:
 */

#include <string>

#include "WindowsUtils.hpp"
#include "MIAException.hpp"
#include "Error.hpp"


namespace windows_utils
{

	void terminalCommand(const std::string& command)
	{
	#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
		system(command.c_str());
	#else
		throw error::MIAException(error::ErrorCode::Windows_Only_Feature);
	#endif
	}
} // namespace windows_utils
