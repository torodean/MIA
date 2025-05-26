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
#include <cstdlib>

// The associated header file.
#include "MIASequencer.hpp"

// Used for error handling.
#include "Paths.hpp"
#include "MIAException.hpp"
// Used for string manipulation and parsing.
#include "StringUtils.hpp"
// Used for sleep.
#include "Timing.hpp"

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
                                CommandOption::commandOptionType::boolOption),
    sequenceNameOpt("-s", "--sequence", "Run a sequence, then exit.",
                                CommandOption::commandOptionType::stringOption)
{ };


void MIASequencer::initialize(int argc, char* argv[])
{
    try
    {    
        MIAApplication::initialize(argc, argv);

        // Set the values from the command line arguments.
        testOpt.getOptionVal<bool>(argc, argv, testMode);
        sequenceNameOpt.getOptionVal<std::string>(argc, argv, sequenceName);
        
        // Set and load the config file.
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
                // TODO - check for duplicate sequences.
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
        action.timeValue = std::stoi(value);
    }
    else if (key == "DELAY") 
    {
        action.action = SequenceActionType::DELAY;
        action.timeValue = std::stoi(value);
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
              << sequenceNameOpt.getHelp() << std::endl
              << testOpt.getHelp() << std::endl
              << std::endl;
}


bool MIASequencer::CompleteSequence::isValid()
{
    return !name.empty() && !actions.empty();
}

void MIASequencer::CompleteSequence::clear()
{
    name.clear();
    delayTime = 1000; // The default value.
    actions.clear();
    return;
}

void MIASequencer::CompleteSequence::performActions(MIA_system::VirtualKeyStrokes& keys, 
                                                    bool testMode)
{
    for (auto& action : actions)
    {
        // Update the delayTime if performActions returns an int.
        if (auto newDelay = action.performAction(keys))
            delayTime = *newDelay;
        
        MIA_system::sleepMilliseconds(delayTime);
    }
}

std::optional<int> MIASequencer::SequenceAction::performAction(MIA_system::VirtualKeyStrokes& keys, 
                                                               bool testMode)
{
    switch(action)
    {
        case SequenceActionType::TYPE:
            keys.type(strToType);
            break;
        case SequenceActionType::SLEEP:
            MIA_system::sleepMilliseconds(timeValue);
            break;
        case SequenceActionType::MOVEMOUSE:
            keys.moveMouseTo(coords.x, coords.y);
            break;
        case SequenceActionType::CLICK:
            switch(click)
            {
                case MIA_system::VirtualKeyStrokes::ClickType::LEFT_CLICK:
                    keys.mouseClick(click);
                    break;
                case MIA_system::VirtualKeyStrokes::ClickType::RIGHT_CLICK:
                    keys.mouseClick(click);
                    break;
                default:
                    // Do nothing...
                    break;
            }
            break;
        case SequenceActionType::DELAY:
            return timeValue;
        default:
            // Do nothing...
            break;
    }
}


void MIASequencer::runSequence(const std::string& sequenceName)
{
    auto it = sequences.find(sequenceName);
    if (it != sequences.end())
        it->second.performActions(keys, testMode);
    else
        std::cout << "Sequence not found/loaded: " << sequenceName << std::endl;
}


void MIASequencer::defaultFrontEnd()
{
    std::string input;
    // Loop over the default interface.
    while (true) 
    {
        std::cout << "Enter a sequence name to run: ";
        std::getline(std::cin, input);

        if (input.empty()) 
            continue;
            
        // Run the sequence.
        runSequence(input);
    }
}


int MIASequencer::run()
{
    // This would indicate no sequenceNameOpt specified.
    if (sequenceName.empty())
    {
        defaultFrontEnd();
    }
    else
    {
        runSequence(sequenceName);
        return 0;
    }
    return 0;
}












