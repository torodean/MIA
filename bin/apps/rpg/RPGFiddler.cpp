/**
 * @file RPGFiddler.cpp
 * @author Antonius Torode
 * @date 05/24/2025
 * Description: Implementation of the RPGFiddler app.
 */

#include <iostream>

// The associated header file.
#include "RPGFiddler.hpp"

// Used for error handling.
#include "Paths.hpp"
#include "MIAException.hpp"

#include "Constants.hpp"
// Used for changing terminal colors.
#include "TerminalColors.hpp"
#include "RPGSimulator.hpp"
#include "DataLoader.hpp"

#include "FileUtils.hpp"


RPGFiddler::RPGFiddler() : 
    saveFileOpt("-s", "--save", "The file to save the player state to (default = " +
                paths::getDefaultConfigDirToUse() + "/" + defaultSaveFile,
                CommandOption::commandOptionType::STRING_OPTION)                   
{ };


void RPGFiddler::initialize(int argc, char* argv[])
{
    try
    {    
        MIAApplication::initialize(argc, argv);        
        
        // Set the values from the command line arguments.
        saveFileOpt.getOptionVal<std::string>(argc, argv, fullSaveFilePath);        
        if (fullSaveFilePath.empty())
            fullSaveFilePath = paths::getDefaultConfigDirToUse() + "/" + defaultSaveFile;
        else if (fullSaveFilePath[0] != '/')
            fullSaveFilePath = paths::getDefaultConfigDirToUse() + "/" + fullSaveFilePath;            
    
        // Load the RPG configuration.
        std::string configDir = "/home/awtorode/git/MIA/bin/libs/rpg/data/";
        if (!rpg::DataLoader::getInstance().initialize(configDir)) 
        {
            MIA_THROW(error::ErrorCode::Catastrophic_Failure, "Failed to initialize registries.");
        }
        
        // Load the saved player data or initialize fresh rpg data for testing.
        if (files::fileExists(fullSaveFilePath))
            player.loadFromFile(fullSaveFilePath);
        else
            rpg_sim::setupSimulator(player);
    }
    catch (const error::MIAException& ex)
    {
        std::cerr << "Error during RPGFiddler::initialize: " << ex.what() << std::endl;
    }
}


void RPGFiddler::printHelp() const
{
    MIAApplication::printHelp();
    
    // This is a dump of the help messages used by the various command options.
    std::cout << "RPGFiddler specific options:" << std::endl
              << saveFileOpt.getHelp() << std::endl
              << std::endl;
}


int RPGFiddler::run()
{
    LOG_METHOD_CALL(); // Used for testing log file calls.
    
    rpg_sim::runSimulator(player, fullSaveFilePath);
    
    return constants::SUCCESS;
}
