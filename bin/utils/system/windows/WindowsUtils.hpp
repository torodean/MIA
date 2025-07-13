/**
 * @file WindowsUtils.hpp
 * @author Antonius Torode
 * @date 03/07/2021
 * Description:
 */
#pragma once

/**
 * This namespace is for utility features related to Windows.
 */
namespace windows_utils
{
    /**
     * Runs a cmd terminal command.
     * @param command[std::string&] - A reference to the command to run.
     */
    void terminalCommand(const std::string& command);
}
