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
// Used for config type.
#include "Constants.hpp"

using StringUtils::stringContainsChar;
using StringUtils::getBeforeChar;
using StringUtils::getAfterChar;
using StringUtils::contains;
using StringUtils::trim;
using virtual_keys::VirtualKeyStrokes;

MIASequencer::MIASequencer() : 
    config(defaultSequencesFile, constants::ConfigType::RAW_LINES),
    sequencesFileOpt("-c", "--config", "Specify a config file to use (default = " +
                                paths::getDefaultConfigDirToUse() + "/MIASequences.MIA)",
                                CommandOption::commandOptionType::stringOption),
    testOpt("-t", "--test", "Enables test mode. This mode will only output the sequence to terminal.",
                                CommandOption::commandOptionType::boolOption),
    sequenceNameOpt("-s", "--sequence", "Run a sequence, then exit.",
                                CommandOption::commandOptionType::stringOption),
	loopModeOpt("-L", "--loop", "Loop over the activated sequence indefinitely.",
								CommandOption::commandOptionType::boolOption),
	printSequencesOpt("-P", "--list", "Print a list of all valid sequences when ran.",
								CommandOption::commandOptionType::boolOption)
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
    CompleteSequence sequence;
    
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
                // TODO - report error here.
                sequence.clear();
            }
        }
        else // Everything else is an action or invalid.
        {
            SequenceAction action = createAction(key, value);
            if (action.isValid())
                sequence.actions.push_back(action);
        }
    }
}


MIASequencer::SequenceAction MIASequencer::createAction(std::string key, std::string value)
{
    SequenceAction action;
    if (key == "TYPE") 
    {
        action.actionType = SequenceActionType::TYPE;
        action.strToType = value;
    }
    else if (key == "SLEEP") 
    {
        action.actionType = SequenceActionType::SLEEP;
        action.timeValue = std::stoi(value);
    }
    else if (key == "DELAY") 
    {
        action.actionType = SequenceActionType::DELAY;
        action.timeValue = std::stoi(value);
    }
    else if (key == "MOVEMOUSE") 
    {
        action.actionType = SequenceActionType::MOVEMOUSE;
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
        action.actionType = SequenceActionType::CLICK;
        std::string trimmedClickString = trim(value);
        action.click = VirtualKeyStrokes::stringToClickType(trimmedClickString);
    }
    else if (key == "PRESS") 
    {
        action.actionType = SequenceActionType::PRESS;
        std::string trimmedPressString = trim(value);
        action.press = VirtualKeyStrokes::stringToSpecialButton(trimmedPressString);
    }
    else
    {    
        action.actionType = SequenceActionType::UNKNOWN;
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
    std::cout << "MIATemplate specific options:" << std::endl
              << sequencesFileOpt.getHelp() << std::endl
              << sequenceNameOpt.getHelp() << std::endl
              << testOpt.getHelp() << std::endl
			  << loopModeOpt.getHelp() << std::endl
			  << printSequencesOpt.getHelp() << std::endl
              << std::endl;
}


bool MIASequencer::SequenceAction::isValid()
{
    switch (actionType) 
    {
        case SequenceActionType::TYPE:
            return !strToType.empty();

        case SequenceActionType::SLEEP:
        case SequenceActionType::DELAY:
            return timeValue >= 0;

        case SequenceActionType::MOVEMOUSE:
            return true;  // No restriction on coords

        case SequenceActionType::CLICK:
            return click != VirtualKeyStrokes::ClickType::UNKNOWN;

        case SequenceActionType::PRESS:
            return press != VirtualKeyStrokes::SpecialButton::UNKNOWN;

        default:
            return false;
    }
}


bool MIASequencer::CompleteSequence::isValid()
{
    return !name.empty() && !actions.empty();
}


void MIASequencer::CompleteSequence::clear()
{
    name.clear();
    delayTime = 1000; // The default value.
    listenerKeyCode = '\0';
    actions.clear();
    return;
}


std::optional<int> MIASequencer::SequenceAction::performAction(VirtualKeyStrokes& keys, 
                                                               bool testMode)
{
    if (testMode)
    {
        dump();
    }
    else
    {
        switch(actionType)
        {
            case SequenceActionType::TYPE:
                keys.type(strToType);
                break;
            case SequenceActionType::SLEEP:
                timing::sleepMilliseconds(timeValue);
                break;
            case SequenceActionType::MOVEMOUSE:
                keys.moveMouseTo(coords.x, coords.y);
                break;
            case SequenceActionType::CLICK:
                if (click != VirtualKeyStrokes::ClickType::UNKNOWN)
                    keys.mouseClick(click);                    
                break;
            case SequenceActionType::PRESS:
                if (press != VirtualKeyStrokes::SpecialButton::UNKNOWN)
                    keys.pressSpecialButton(press);                    
                break;
            case SequenceActionType::DELAY:
                return timeValue;
            default:
                // Do nothing...
                break;
        }
    }
    
    if (testMode)
        std::cout << std::endl;
        
    return std::nullopt;
}


void MIASequencer::CompleteSequence::performActions(VirtualKeyStrokes& keys, 
                                                    bool testMode)
{
    for (auto& action : actions)
    {
        if (testMode)
        {
            action.dump();
            std::cout << " ";
        }
        else
        {
            // Update the delayTime if performActions returns an int.
            if (auto newDelay = action.performAction(keys))
                delayTime = *newDelay;
            
            timing::sleepMilliseconds(delayTime);
        }
    }
    
    if (testMode)
        std::cout << std::endl;
}


void MIASequencer::SequenceAction::dump() const 
{
    switch (actionType) 
    {
        case SequenceActionType::TYPE:
            std::cout << "TYPE:" << strToType;
            break;
            
        case SequenceActionType::SLEEP:
            std::cout << "SLEEP:" << timeValue << "ms";
            break;
            
        case SequenceActionType::MOVEMOUSE:
            std::cout << "MOVEMOUSE:" << coords.x << "," << coords.y;
            break;
            
        case SequenceActionType::CLICK:
            std::cout << "CLICK:" << VirtualKeyStrokes::clickTypeToString(click);
            break;
            
        case SequenceActionType::PRESS:
            std::cout << "PRESS:" << VirtualKeyStrokes::specialButtonToString(press);
            break;
            
        case SequenceActionType::DELAY:
            std::cout << "DELAY:" << timeValue << "ms";
            break;
                        
        default:
            std::cout << "UNKNOWN";
            break;
    }
    std::cout << std::flush;
}


void MIASequencer::CompleteSequence::dump() const 
{
    std::cout << " -- { " << name << ", DELAY=";
    std::cout << delayTime << "ms";
    
    if (listenerKeyCode != '\0')
        std::cout << ", LISTEN=" << listenerKeyCode;

    for (const auto& action : actions) 
    {
        std::cout << ", ";
        action.dump();
    }
    std::cout << " }" << std::endl;
}


void MIASequencer::runSequence(CompleteSequence& sequence)
{
    sequence.performActions(keys, testMode);
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
            CompleteSequence& sequence = it->second;
            
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












