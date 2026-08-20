/**
 * @file MIASequencer.cpp
 * @author Antonius Torode
 * @date 05/24/2025
 * @brief Implementation of the MIASequencer app.
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

// Used for error handling and configuration.
#include "Paths.hpp"
#include "MIAException.hpp"
// Used for string manipulation and parsing.
#include "StringUtils.hpp"
// Used for sleep.
#include "Timing.hpp"
// Used for config type.
#include "Constants.hpp"

using StringUtils::stringContainsChar;
using StringUtils::getBeforeChar;
using StringUtils::getAfterChar;
using StringUtils::contains;
using StringUtils::trim;
using StringUtils::delimiterString;


MIASequencer::MIASequencer() : 
    config(defaultSequencesFile, constants::ConfigType::RAW_LINES),
    sequencesFileOpt("-c", "--config", "Specify a config file to use (default = " +
                     paths::getDefaultConfigDirToUse() + "/" + defaultSequencesFile + ")",
                     CommandOption::commandOptionType::STRING_OPTION),
    testOpt("-t", "--test", "Enables test mode. This mode will only output the sequence to terminal.",
            CommandOption::commandOptionType::BOOL_OPTION),
    sequenceNameOpt("-s", "--sequence", "Run a sequence, then exit.",
                    CommandOption::commandOptionType::STRING_OPTION),
    loopModeOpt("-L", "--loop", "Loop over the activated sequence indefinitely.",
                CommandOption::commandOptionType::BOOL_OPTION),
    printSequencesOpt("-P", "--list", "Print a list of all valid sequences when ran.",
                      CommandOption::commandOptionType::BOOL_OPTION)
{ };


void MIASequencer::initialize(int argc, char* argv[])
{
    try
    {    
        MIAApplication::initialize(argc, argv);

        // Set the values from the command line arguments.
        testOpt.getOptionVal<bool>(argc, argv, testMode);
        sequenceNameOpt.getOptionVal<std::string>(argc, argv, sequenceName);
        loopModeOpt.getOptionVal<bool>(argc, argv, loopMode);
        printSequencesOpt.getOptionVal<bool>(argc, argv, printSequences);
        
        // Set and load the config file.
        std::string sequencesFile = defaultSequencesFile;
        sequencesFileOpt.getOptionVal<std::string>(argc, argv, sequencesFile);
        config.setConfigFileName(sequencesFile, constants::ConfigType::RAW_LINES); // handles config.initialize().
    
        // By this point, verbose, debug, etc are all set.
        listener.setContext(getContext());
    }
    catch (const error::MIAException& ex)
    {
        std::cerr << "Error during MIASequencer::initialize: " << ex.what() << std::endl;
    }
    
    loadConfig();
}


void MIASequencer::loadConfig()
{    
    sequences::CompleteSequence sequence;
    
    // The config stores all non-comment and non-empty lines from the config file.
    std::vector<std::string> lines = config.getRawLines();
    for (auto& line : lines)
    {
        std::string key, value;
        // See if the line is a key/value pair or contains the end of sequence marker.
        if (stringContainsChar(line, '='))
        {
            // Get the key value on the line.
            key = getBeforeChar(line, '=');
                
            // Get the value of the key.
            value = getAfterChar(line, '=');  
        }
        else
        {
            key = trim(line);
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
        else if (key == "LISTEN")
        {
            sequence.listenerKeyCode = value[0];
        }
        else if (key == "ENDOFSEQUENCE") // Complete the sequence.
        {
            if (sequence.isValid())
            {
                std::string seqName = trim(sequence.name);
                if (getVerboseMode())
                {
                    std::cout << "Adding sequence to sequences map: " << seqName << std::endl;
                    sequence.dump();
                }
                // TODO - check for duplicate sequences.
                sequences[seqName] = sequence;
                sequence.clear();
            }
            else // Invalid sequence so restart and ignore this one.
            {
                std::string invalidSequenceErr = "ERROR: Invalid configuration for the sequence " 
                                               + sequence.name
                                               + " Config file in question is: "
                                               + config.getConfigFileFullPath();
                MIA_THROW(error::ErrorCode::Invalid_Sequence_Option, invalidSequenceErr);
                sequence.clear();
            }
        }
        else // Everything else is an action or invalid.
        {
            sequences::SequenceAction action = createAction(key, value);
            if (action.isValid())
                sequence.actions.push_back(action);
        }
    }
}


sequences::SequenceAction MIASequencer::createAction(std::string key, std::string value)
{
    sequences::SequenceAction action;
    if (key == "TYPE")
    {
        action.actionType = sequences::SequenceActionType::TYPE;
        action.strToType = value;
    }
    else if (key == "SLEEP")
    {
        action.actionType = sequences::SequenceActionType::SLEEP;
        action.timeValue = std::stoi(value);
    }
    else if (key == "DELAY")
    {
        action.actionType = sequences::SequenceActionType::DELAY;
        action.timeValue = std::stoi(value);
    }
    else if (key == "MOVEMOUSE")
    {
        action.actionType = sequences::SequenceActionType::MOVEMOUSE;
        math::Coordinate coords(0,0);
        if (stringContainsChar(value, ','))
        {
            coords.x = std::stoi(getBeforeChar(value, ','));
            coords.y = std::stoi(getAfterChar(value, ','));
        }
        action.coords = coords;
    }
    else if (key == "CLICK")
    {
        action.actionType = sequences::SequenceActionType::CLICK;
        std::string trimmedClickString = trim(value);
        action.click = virtual_keys::stringToClickType(trimmedClickString);
    }
    else if (key == "PRESS")
    {
        action.actionType = sequences::SequenceActionType::PRESS;
        std::string trimmedPressString = trim(value);
        action.press = virtual_keys::stringToSpecialButton(trimmedPressString);
    }
    else if (key == "TYPEHOLD")
    {
        action.actionType = sequences::SequenceActionType::TYPEHOLD;
        std::string trimmedTypeHoldString = trim(value);
        std::vector<std::string> typeHoldVec = delimiterString(trimmedTypeHoldString, ";");
        action.strToType = typeHoldVec[0];
        action.timeValue = std::stoi(typeHoldVec[1]);
    }
    else if (key == "PRESSHOLD")
    {
        action.actionType = sequences::SequenceActionType::PRESSHOLD;
        std::string trimmedPressHoldString = trim(value);
        std::vector<std::string> pressHoldVec = delimiterString(trimmedPressHoldString, ";");
        action.press = virtual_keys::stringToSpecialButton(pressHoldVec[0]);
        action.timeValue = std::stoi(pressHoldVec[1]);
    }
    else if (key == "CLICKHOLD")
    {
        action.actionType = sequences::SequenceActionType::CLICKHOLD;
        std::string trimmedClickHoldString = trim(value);
        std::vector<std::string> clickHoldVec = delimiterString(trimmedClickHoldString, ";");
        action.click = virtual_keys::stringToClickType(clickHoldVec[0]);
        action.timeValue = std::stoi(clickHoldVec[1]);
    }
    else if (key == "PRESSRANDNUM")
    {
        action.actionType = sequences::SequenceActionType::PRESSRANDNUM;
        math::Coordinate coords(0,0);
        if (stringContainsChar(value, ';'))
        {
            coords.x = std::stoi(getBeforeChar(value, ';'));
            coords.y = std::stoi(getAfterChar(value, ';'));
        }
        action.coords = coords;
    }
    else
    {    
        action.actionType = sequences::SequenceActionType::UNKNOWN;
    }
    
    if (getVerboseMode())
    {
        std::cout << "Action created: ";
        action.dump();
        std::cout << std::endl;
    }
    
    return action;
}


void MIASequencer::printHelp() const
{
    MIAApplication::printHelp();
    
    // This is a dump of the help messages used by the various command options.
    std::cout << "MIASequencer specific options:" << std::endl
              << sequencesFileOpt.getHelp() << std::endl
              << sequenceNameOpt.getHelp() << std::endl
              << testOpt.getHelp() << std::endl
              << loopModeOpt.getHelp() << std::endl
              << printSequencesOpt.getHelp() << std::endl
              << std::endl;
}


void MIASequencer::runSequence(sequences::CompleteSequence& sequence)
{
    sequence.performActions(keys, getVerboseMode(), testMode);
}


void MIASequencer::runSequence(const std::string& sequenceName)
{
    auto it = sequences.find(sequenceName);
    if (it != sequences.end())
        runSequence(it->second);
    else
        std::cout << "Sequence not found/loaded: " << sequenceName << std::endl;
}


void MIASequencer::printSequenceList(std::ostream& out)
{
    out << "All valid sequences listed below:" << std::endl;
    for(const auto& sequence : sequences)
    {
        out << " -> " << sequence.first << std::endl;
    }
}


void MIASequencer::defaultFrontEnd()
{
    std::string input;
    // Loop over the default interface.
    while (true) 
    {
        std::cout << "Enter a sequence to perform: ";
        std::getline(std::cin, input);

        if (input.empty()) 
            continue;
            
        // Run the sequence.
        do { runSequence(input); } while (loopMode);
    }
}


int MIASequencer::run()
{
    if (printSequences)
        printSequenceList();
    
    // This would indicate no sequenceNameOpt specified.
    if (sequenceName.empty())
    {
        defaultFrontEnd();
    }
    else
    {
        std::cout << "Activating sequence: " << sequenceName << std::endl;
        auto it = sequences.find(sequenceName);
        if (it != sequences.end())
        {
            sequences::CompleteSequence& sequence = it->second;
            
            // If this is false, then a LISTEN value is not set for this sequence.
            if (sequence.listenerKeyCode != '\0')
            {
                std::cout << "Press the '" << sequence.listenerKeyCode 
                          << "' key to start/stop the sequence." << std::endl;
                          
                listener.setKeyCode(sequence.listenerKeyCode);
                listener.initialize();
                listener.start();
                
                do // Perform the sequence (and loop if needed). 
                {
                    // Start with the sequence off until the condition is met.
                    while (listener.isConditionMet())
                    {
                        runSequence(sequence);
                        if (!loopMode) break;
                    }
                } while (loopMode);
            }
            else
            {
                do // Perform the sequence (and loop if needed). 
                {
                    runSequence(sequence);
                } while (loopMode);
            }

        }
        else
        {
            std::cout << "Sequence not found/loaded: " << sequenceName << std::endl;
        }
    }
    return constants::SUCCESS;
}

