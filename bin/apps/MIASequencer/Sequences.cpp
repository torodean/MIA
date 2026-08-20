/**
 * @file Sequences.cpp
 * @author Antonius Torode
 * @date 05/24/2025
 * @brief Implementation of the MIASequencer app.
 */

#include <iostream>
#include <algorithm>

// The associated header file.
#include "Sequences.hpp"

// Used for sleep.
#include "Timing.hpp"

namespace sequences
{
    bool SequenceAction::isValid()
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
                return click != virtual_keys::ClickType::UNKNOWN;

            case SequenceActionType::PRESS:
                return press != virtual_keys::SpecialButton::UNKNOWN;

            case SequenceActionType::TYPEHOLD:
                return !strToType.empty() && timeValue > 0;

            case SequenceActionType::PRESSHOLD:
                return press != virtual_keys::SpecialButton::UNKNOWN  && timeValue > 0;

            case SequenceActionType::CLICKHOLD:
                return click != virtual_keys::ClickType::UNKNOWN  && timeValue > 0;
                
            case SequenceActionType::PRESSRANDNUM:
                return (coords.x >= 0 && coords.y >= 0) && (coords.x <= 9 && coords.y <= 9);

            default:
                return false;
        }
    }


    bool CompleteSequence::isValid()
    {
        return !name.empty() && !actions.empty();
    }


    void CompleteSequence::clear()
    {
        name.clear();
        delayTime = 1000; // The default value.
        listenerKeyCode = '\0';
        actions.clear();
        return;
    }


    std::optional<int> SequenceAction::performAction(virtual_keys::VirtualKeyStrokes& keys,
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
                case SequenceActionType::PRESSRANDNUM:
                    if (isValid())
                        virtual_keys::pressRandomNumber(keys, coords.x, coords.y, timeValue, verboseMode);
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


    void CompleteSequence::performActions(virtual_keys::VirtualKeyStrokes& keys, 
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


    void SequenceAction::dump() const 
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
                std::cout << "CLICK:" << virtual_keys::clickTypeToString(click);
                break;
                
            case SequenceActionType::PRESS:
                std::cout << "PRESS:" << virtual_keys::specialButtonToString(press);
                break;
                
            case SequenceActionType::TYPEHOLD:
                std::cout << "TYPE:" << strToType
                          << ";HOLD:" << timeValue << "ms";
                break;
                
            case SequenceActionType::PRESSHOLD:
                std::cout << "PRESS:" << virtual_keys::specialButtonToString(press)
                          << ";HOLD:" << timeValue << "ms";
                
            case SequenceActionType::CLICKHOLD:
                std::cout << "CLICK:" << virtual_keys::clickTypeToString(click)
                          << ";HOLD:" << timeValue << "ms";
                break;
                
            case SequenceActionType::DELAY:
                std::cout << "DELAY:" << timeValue << "ms";
                break;
                
            case SequenceActionType::PRESSRANDNUM:
                std::cout << "PRESSRANDNUM:" << coords.x << ";" << coords.y;
                break;
                            
            default:
                std::cout << "UNKNOWN";
                break;
        }
        std::cout << std::flush;
    }


    void CompleteSequence::dump() const 
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

} // namespace sequences
