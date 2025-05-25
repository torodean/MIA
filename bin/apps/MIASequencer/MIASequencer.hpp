/**
 * File: MIASequencer.hpp
 * Author: Antonius Torode
 * Created on: 12/26/2019
 * Description: A sequencer for processing and executing MIASequences from file.
 *              This component supports loading, parsing, and triggering predefined
 *              input sequences including coordinates and actions.
 */
#pragma once

#include <string>
#include <vector>
#include <map>

// Used for the configuration.
#include "MIAConfig.hpp"
// Used for common app setup.
#include "MIAApplication.hpp"
// Used for command options.
#include "CommandOption.hpp"

#include "Constants.hpp"

//Used for virtual key strokes.
#include "VirtualKeyStrokes.hpp"

/**
 * @class MIASequencer
 * @brief A component for executing user-defined input automation sequences.
 *
 * The MIASequencer is responsible for loading, parsing, and executing sequences
 * of simulated user input defined in a configuration file (default: MIASequences.MIA).
 * These sequences can automate keyboard and mouse interactions by specifying actions
 * like typing, mouse movement, and clicking at specified screen coordinates.
 */
class MIASequencer : public MIAApplication
{
public:

    /**
     * These are the various actions which the sequencer supports.
     */
    enum SequenceActionType
    {
        UNKNOWN,     ///< Unknown action - do nothing.
        TYPE,        ///< This will type a sequence of characters.
        SLEEP,       ///< This will wait/pause some time.
        MOVEMOUSE,   ///< This will move the mouse to a specific cordinate.
        CLICK        ///< This will perform a click with the mouse.
    };
    
    /**
     * This struct represents storage for a valid sequencer action. This action
     * defines a type, which specifies what the action will do, and then containers
     * for each type of data needed for the various actions. The intention is to only
     * fill the data type associated with the type needed for the action.
     */
    struct SequenceAction
    {
        SequenceActionType action;      ///< An action type (what to perform).
        int sleepTime;                  ///< Timing information (for pause).
        constants::Coordinate coords;   ///< Coordinates (for moving mouse).
        std::string strToType;          ///< A string (for typing).
        MIA_system::VirtualKeyStrokes::ClickType click; ///< A click type (for mouse input).
        
        /**
         * @brief Executes the stored action based on its type and associated data.
         *
         * This function performs the action defined by the `action` field using
         * the relevant data members (i.e., coordinates, time, string, click type).
         * Only the data appropriate to the action type is used.
         */
        static void performAction();
    };
    
    /// A list of sequencer actions forming a complete sequence.
    using std::vector<SequenceAction> SequenceActions;
    
    /**
     * @brief Represents a fully defined input sequence with a name, timing, and actions.
     *
     * A CompleteSequence contains:
     * - A unique name identifying the sequence,
     * - A delay time in milliseconds between each action,
     * - An ordered list of SequenceAction items to execute.
     */
    struct CompleteSequence
    {
        std::string name;        ///< The name of this sequence.
        int delayTime{1000};     ///< The time between each action (ms).
        SequenceActions actions; ///< All actions in this sequence.
        
        /// Returns true if this is a valid sequence.
        void isValid();
        /// Clear this object.
        void clear();
        
        /**
         * @brief Executes all actions in the sequence in order, applying the defined delay between each.
         * Iterates through the `actions` list and calls `performAction()` on each entry,
         * pausing for `delayTime` milliseconds between actions.
         */
        static void performActions();
    };
    
    /// Maps sequence names to their corresponding list of actions.
    using std::unordered_map<std::string, CompleteSequence> sequenceList;

    /**
     * The main constructor of the MIASequencer class. This will construct the command options.
     */
    MIASequencer();

    /// Default destructor.
    ~MIASequencer() = default;
    
    /**
     * This will initialize this class by parsing command line arguments for configuration options
     * and then loading the config file if provided.
     *
     * @param argc Number of command line arguments.
     * @param argv Array of command line argument strings.
     */
    void initialize(int argc, char* argv[]) override;
    
    /// Runs the MIASequencer application.
    int run() override;

protected:

    /**
     * Prints help info for optional config file.
     */
    virtual void printHelp() const;

private:
    
    /**
     * This will create a SequenceAction from a key-value pair.
     * @param key[std::String] - The key defining the action type.
     * @param value[std::String] - The value defining the data needed to perform the action.
     * @return [SequenceAction] - Returns the constructed SequenceAction.  
     */
    SequenceAction createAction(std::String key, std::string value);

    /**
     * This will load in the configuration file and load the sequences.
     */
    void loadConfig();
		
    // Command options for this app.
    CommandOption sequencesFileOpt;  ///< Used for loading a custom sequences file.
    CommandOption testOption;        ///< Used for enabling test mode.
    
    /// The name of the defauilt config file that this class uses.
    std::string defaultSequencesFile{"MIASequences.MIA"};
    
    /**
     * The configuration loader for this app. The configuration for this app uses the
     * constants::ConfigType::RAW_LINES format since it also loads the sequencer fields
     * in which need custom parsing and interpretation.
     */
    MIA_System::MIAConfig config;
    
    /// The complete list of sequences.
    sequenceList sequences;
    
}; // class MIASequencer
