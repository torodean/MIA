/**
 * File: RPGFiddler.cpp
 * Author: Antonius Torode
 * Date: 05/24/2025
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

#include "DataLoader.hpp"

RPGFiddler::RPGFiddler()                      
{ };


void RPGFiddler::initialize(int argc, char* argv[])
{
    try
    {    
        MIAApplication::initialize(argc, argv);
    
        std::string configDir = "/home/awtorode/git/MIA/bin/libs/rpg/data/";
        if (!rpg::DataLoader::getInstance().initialize(configDir)) 
        {
            MIA_THROW(error::ErrorCode::Catastrophic_Failure, "Failed to initialize registries.");
        }
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
              << "   - None yet..." << std::endl
              << std::endl;
}


int RPGFiddler::run()
{
    LOG_METHOD_CALL(); // Used for testing log file calls.
    
    player.getWallet().dump(); std::cout << std::endl;
    if (const auto* copper = currency::CurrencyRegistry::getInstance().getCurrencyByName("Copper Coin"))
        player.getWallet().addCurrency(*copper, 42);    
    player.getWallet().dump(); std::cout << std::endl;
    if (const auto* copper = currency::CurrencyRegistry::getInstance().getCurrencyByName("Copper Coin"))
        player.getWallet().addCurrency(*copper, 7);    
    player.getWallet().dump(); std::cout << std::endl;
    if (const auto* copper = currency::CurrencyRegistry::getInstance().getCurrencyByName("Gold Coin"))
        player.getWallet().addCurrency(*copper, 3);    
    player.getWallet().dump(); std::cout << std::endl;
    if (const auto* copper = currency::CurrencyRegistry::getInstance().getCurrencyByName("Copper Coin"))
        player.getWallet().removeCurrency(*copper, 25);    
    player.getWallet().dump(); std::cout << std::endl;
    
    return constants::SUCCESS;
}
