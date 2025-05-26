/**
 * File: MIASequencer.cpp
 * Author: Antonius Torode
 * Date: 05/24/2025
 * Description: Implementation of the MIASequencer app.
 */

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <algorithm>

// The associated header file.
#include "MIASequencer.hpp"

// Used for error handling.
#include "Paths.hpp"
#include "MIAException.hpp"
// Used for string manipulation and parsing.
#include "StringUtils.hpp"

using types::stringContainsChar;
using types::getBeforeChar;
using types::getAfterChar;
using MIA_system::VirtualKeyStrokes;

MIASequencer::MIASequencer() : 
    config(defaultSequencesFile, constants::ConfigType::RAW_LINES),
    sequencesFileOpt("-c", "--config", "Specify a config file to use (default = " +
                                paths::getDefaultConfigDirToUse() + "/MIASequences.MIA)",
                                CommandOption::commandOptionType::stringOption),
    testOpt("-t", "--test", "Enables test mode. This mode will only output the sequence to terminal.",
                                CommandOption::commandOptionType::boolOption)
{ };


void MIASequencer::initialize(int argc, char* argv[])
{
    try
    {    
        MIAApplication::initialize(argc, argv);

        // Set the values from the command line arguments.
        testOpt.getOptionVal<bool>(argc, argv, testMode);
        
        std::string sequencesFile = defaultSequencesFile;
        sequencesFileOpt.getOptionVal<std::string>(argc, argv, sequencesFile);
        config.setConfigFileName(sequencesFile, constants::ConfigType::RAW_LINES); // handles config.initialize().
    }
    catch (const error::MIAException& ex)
    {
        std::cerr << "Error during MIASequencer::initialize: " << ex.what() << std::endl;
    }
    
    loadConfig();
}


void MIASequencer::loadConfig()
{
    // TODO - finish this method.
    
    CompleteSequence sequence;
    
    // The config stores all non-comment and non-empty lines from the config file.
    std::vector<std::string> lines = config.getRawLines();
    for (auto& line : lines)
    {
        std::string key, value;
        if (stringContainsChar(line, '='))
        {
            // Get the key value on the line.
            key = getBeforeChar(line, '=');
            // Get the value of the key.
            value = getAfterChar(line, '=');  
        }
        else
        {
            key = line;
        }
        
        // Check if this is the start of end of a sequence.
        if (key == "SEQUENCENAME")
        {
            sequence.name = value;
        }
        else if (key == "DELAY")
        {
            sequence.delayTime = std::stoi(value);
        }
        else if (key == "ENDOFSEQUENCE") // Complete the sequence.
        {
            if (sequence.isValid())
            {
                // TODO - check for pre-existing sequences.
                sequences[sequence.name] = sequence;
                sequence.clear();
            }
            else // Invalid sequence so restart and ignore this one.
            {
                // TODO - report error here.
                sequence.clear();
            }
        }
        else // Everything else is an action or invalid.
        {
            sequence.actions.push_back(createAction(key, value));
        }
    }    
}


MIASequencer::SequenceAction MIASequencer::createAction(std::string key, std::string value)
{
    SequenceAction action;
    if (key == "TYPE") 
    {
        action.action = SequenceActionType::TYPE;
        action.strToType = value;
    }
    else if (key == "SLEEP") 
    {
        action.action = SequenceActionType::SLEEP;
        action.sleepTime = std::stoi(value);
    }
    else if (key == "MOVEMOUSE") 
    {
        action.action = SequenceActionType::MOVEMOUSE;
        constants::Coordinate coords(0,0);
        if (stringContainsChar(value, ','))
        {
            coords.x = std::stoi(getBeforeChar(value, ','));
            coords.y = std::stoi(getAfterChar(value, ','));
        }
        action.coords = coords;
    }
    else if (key == "CLICK") 
    {
        action.action = SequenceActionType::CLICK;
        action.click = VirtualKeyStrokes::stringToClickType(value);
    }
    else
    {    
        action.action = SequenceActionType::UNKNOWN;
    }
    
    return action;
}


void MIASequencer::printHelp() const
{
    MIAApplication::printHelp();
    
    // This is a dump of the help messages used by the various command options.
    std::cout << "MIATemplate specific options:" << std::endl
              << sequencesFileOpt.getHelp() << std::endl
              << testOpt.getHelp() << std::endl
              << std::endl;
}


bool MIASequencer::CompleteSequence::isValid()
{
    // TODO
    return true;
}

void MIASequencer::CompleteSequence::clear()
{
    // TODO
    return;
}

void MIASequencer::CompleteSequence::performActions()
{
    // TODO
    return;
}

void MIASequencer::SequenceAction::performAction()
{
    // TODO
    return;
}

int MIASequencer::run()
{
    // TODO
    return 0;
}
