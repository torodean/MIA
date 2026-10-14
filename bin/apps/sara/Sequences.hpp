/**
 * @file Sequences.hpp
 * @author Antonius Torode
 * @date 12/26/2019
 * @brief Defines methods related to storing and tracking sequences for the MIASequencer.
 */
#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include <limits>

// Used for Coordinate type.
#include "MathTypes.hpp"
//Used for virtual key strokes.
#include "VirtualKeyStrokes.hpp"

namespace sequences
{
    /**
     * These are the various actions which the sequencer supports.
     */
    enum SequenceActionType
    {
        UNKNOWN,     ///< Unknown action - do nothing.
        TYPE,        ///< This will type a sequence of characters.
        SLEEP,       ///< This will wait/pause some time.
        DELAY,       ///< This is the time to wait between each action.
        MOVEMOUSE,   ///< This will move the mouse to a specific cordinate.
        CLICK,       ///< This will perform a click with the mouse.
        PRESS,       ///< This will perform a special button press.
        PRESSRANDNUM,///< This will press a random numered key between two values.
        TYPEHOLD,    ///< This will press a character and hold it for a specified time.
        PRESSHOLD,   ///< This will perform a special button press and hold it for a specified time.
        CLICKHOLD,   ///< This will perform a click with the mouse and hold it for a specified time.
        LISTEN,      ///< This will listen for a key press and stop or restart the sequence.
    };
    
    /**
     * This struct represents storage for a valid sequencer action. This action
     * defines a type, which specifies what the action will do, and then containers
     * for each type of data needed for the various actions. The intention is to only
     * fill the data type associated with the type needed for the action.
     */
    struct SequenceAction
    {
        SequenceActionType actionType{};     ///< An action type (what to perform).
        int timeValue{};                     ///< Timing information (for pause or delay).
        std::string strToType{};             ///< A string (for typing).
        virtual_keys::ClickType click{};     ///< A click type (for mouse input).
        virtual_keys::SpecialButton press{}; ///< A special button for pressing. 
        unsigned int keyCode{};              ///< A generic key code.
        
        /// Coordinates (for moving mouse). Also stores the range for the PRESSRANDNUM option.
        math::Coordinate coords{};
        
        /// Returns true if this is a valid action.
        bool isValid();
        
        /**
         * @brief Outputs a human-readable representation of the acvtion to the standard output.
         * Useful for debugging or verifying the sequence contents before execution.
         */
        void dump() const;
        
        /**
         * @brief Executes the stored action based on its type and associated data.
         * 
         * This function performs the action defined by the `action` field using
         * the relevant data members (i.e., coordinates, time, string, click type).
         * Only the data appropriate to the action type is used.
         *
         * @param keys The object for simulating actions.
         * @param verboseMode If true, enables verbose output.
         * @param testMode Enables test mode (default = false).
         * @return Returns optional new delay time (ms) to use for 
         *     subsequent actions if it needs updated via an action.
         */
        std::optional<int> performAction(virtual_keys::VirtualKeyStrokes& keys,
                                         bool verboseMode = false,
                                         bool testMode = false);
    };
    
    /// A list of sequencer actions forming a complete sequence.
    using SequenceActions = std::vector<SequenceAction>;
    
    /**
     * @brief Represents a fully defined input sequence with a name, timing, and actions.
     *
     * A CompleteSequence contains:
     * - A unique name identifying the sequence,
     * - A delay time in milliseconds between each action,
     * - An ordered list of SequenceAction items to execute.
     * - An optional listenerKeyCode for stop/restarts.
     */
    struct CompleteSequence
    {
        std::string name{};        ///< The name of this sequence.
        int delayTime{1000};       ///< The time between each action (ms).
        SequenceActions actions{}; ///< All actions in this sequence.
        
        /// This stores a key code to listen for in order to stop or restart the sequence.
        char listenerKeyCode{'\0'}; 
        
        /// Returns true if this is a valid sequence.
        bool isValid();
        /// Clear this object.
        void clear();
        
        /**
         * @brief Outputs a human-readable representation of the sequence to the standard output.
         * 
         * Prints the name of the sequence, the delay time between actions, and each action
         * with its specific parameters (e.g., string to type, coordinates, click type, etc.).
         * Useful for debugging or verifying the sequence contents before execution.
         */
        void dump() const;
        
        /**
         * @brief Executes all actions in the sequence in order, applying the defined delay between each.
         *
         * Iterates through the `actions` list and calls `performAction()` on each entry,
         * pausing for `delayTime` milliseconds between actions.
         * 
         * @param keys The object for simulating actions.
         * @param verboseMode If true, enables verbose output.
         * @param testMode Enables test mode (default = false).
         * @param Enables test mode.
         */
        void performActions(virtual_keys::VirtualKeyStrokes& keys, 
                            bool verboseMode = false,
                            bool testMode = false);
    };
    
    /// Maps sequence names to their corresponding list of actions.
    using sequenceList = std::unordered_map<std::string, CompleteSequence>;
    
} // namespace sequences
