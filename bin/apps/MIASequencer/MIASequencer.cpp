/**
 * @file MIASequencer.cpp
 * @author Antonius Torode
 * @date 05/24/2025
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
using StringUtils::delimiterString;
using virtual_keys::VirtualKeyStrokes;

MIASequencer::MIASequencer() : 
    config(defaultSequencesFile, constants::ConfigType::RAW_LINES),
    sequencesFileOpt("-c", "--config", "Specify a config file to use (default = " +
                                paths::getDefaultConfigDirToUse() + "/MIASequences.MIA)",
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
    else if (key == "TYPEHOLD") 
    {
        action.actionType = SequenceActionType::TYPEHOLD;
        std::string trimmedTypeHoldString = trim(value);
        std::vector<std::string> typeHoldVec = delimiterString(trimmedTypeHoldString, ";");
        action.strToType = typeHoldVec[0];
        action.timeValue = std::stoi(typeHoldVec[1]);
    }
    else if (key == "PRESSHOLD") 
    {
        action.actionType = SequenceActionType::PRESSHOLD;
        std::string trimmedPressHoldString = trim(value);
        std::vector<std::string> pressHoldVec = delimiterString(trimmedPressHoldString, ";");
        action.press = VirtualKeyStrokes::stringToSpecialButton(pressHoldVec[0]);
        action.timeValue = std::stoi(pressHoldVec[1]);
    }
    else if (key == "CLICKHOLD") 
    {
        action.actionType = SequenceActionType::CLICKHOLD;
        std::string trimmedClickHoldString = trim(value);
        std::vector<std::string> clickHoldVec = delimiterString(trimmedClickHoldString, ";");
        action.click = VirtualKeyStrokes::stringToClickType(clickHoldVec[0]);
        action.timeValue = std::stoi(clickHoldVec[1]);
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

        case SequenceActionType::TYPEHOLD:
            return !strToType.empty() && timeValue > 0;

        case SequenceActionType::PRESSHOLD:
            return press != VirtualKeyStrokes::SpecialButton::UNKNOWN  && timeValue > 0;

        case SequenceActionType::CLICKHOLD:
            return click != VirtualKeyStrokes::ClickType::UNKNOWN  && timeValue > 0;

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
                                                               bool verboseMode,
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
                keys.type(strToType, 0, verboseMode);
                break;
            case SequenceActionType::SLEEP:
                timing::sleepMilliseconds(timeValue);
                break;
            case SequenceActionType::MOVEMOUSE:
                keys.moveMouseTo(coords.x, coords.y);
                break;
            case SequenceActionType::CLICK:
                if (isValid())
                    keys.mouseClick(click, 0, verboseMode);                    
                break;
            case SequenceActionType::PRESS:
                if (isValid())
                    keys.pressSpecialButton(press, timeValue, verboseMode);                    
                break;
            case SequenceActionType::TYPEHOLD:
                if (isValid())
                    keys.type(strToType, timeValue, verboseMode);
                break;
            case SequenceActionType::PRESSHOLD:
                if (isValid())
                    keys.pressSpecialButton(press, timeValue, verboseMode);                    
                break;
            case SequenceActionType::CLICKHOLD:
                if (isValid())
                    keys.mouseClick(click, timeValue, verboseMode);                    
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
                                                    bool verboseMode,
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
            if (auto newDelay = action.performAction(keys, verboseMode))
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
            
        case SequenceActionType::TYPEHOLD:
            std::cout << "TYPE:" << strToType
                      << ";HOLD:" << timeValue << "ms";
            break;
            
        case SequenceActionType::PRESSHOLD:
            std::cout << "PRESS:" << VirtualKeyStrokes::specialButtonToString(press)
                      << ";HOLD:" << timeValue << "ms";
            
        case SequenceActionType::CLICKHOLD:
            std::cout << "CLICK:" << VirtualKeyStrokes::clickTypeToString(click)
                      << ";HOLD:" << timeValue << "ms";
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












