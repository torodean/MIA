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
#include <optional>
#include <iostream>
#include <limits>

// Used for the configuration.
#include "MIAConfig.hpp"
// Used for common app setup.
#include "MIAApplication.hpp"
// Used for command options.
#include "CommandOption.hpp"
// Used for Coordinate type.
#include "MathTypes.hpp"
//Used for virtual key strokes.
#include "VirtualKeyStrokes.hpp"
#include "KeyListenerTask.hpp"

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
        DELAY,       ///< This is the time to wait between each action.
        MOVEMOUSE,   ///< This will move the mouse to a specific cordinate.
        CLICK,       ///< This will perform a click with the mouse.
        PRESS,       ///< This will perform a special button press.
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
        SequenceActionType actionType{};  ///< An action type (what to perform).
        int timeValue{};                  ///< Timing information (for pause or delay).
        math::Coordinate coords{};        ///< Coordinates (for moving mouse).
        std::string strToType{};          ///< A string (for typing).
        virtual_keys::VirtualKeyStrokes::ClickType click{}; ///< A click type (for mouse input).
        virtual_keys::VirtualKeyStrokes::SpecialButton press{}; ///< A special button for pressing. 
        unsigned int keyCode{};           ///< A generic key code.
        
        /// Returns true if this is a valid action.
        bool isValid();
        
        /**
         * @brief Outputs a human-readable representation of the acvtion to the standard output.
         * Useful for debugging or verifying the sequence contents before execution.
         */
        void dump() const;
        
        /**
         * @brief Executes the stored action based on its type and associated data.
         * This function performs the action defined by the `action` field using
         * the relevant data members (i.e., coordinates, time, string, click type).
         * Only the data appropriate to the action type is used.
         * @param keys[VirtualKeyStrokes& keys] - The object for simulating actions.
         * @param testMode[bool] - Enables test mode (default = false).
         * @return [optional<int>] - Returns optional new delay time (ms) to use for 
         *     subsequent actions if it needs updated via an action.
         */
        std::optional<int> performAction(virtual_keys::VirtualKeyStrokes& keys, 
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
         * Prints the name of the sequence, the delay time between actions, and each action
         * with its specific parameters (e.g., string to type, coordinates, click type, etc.).
         * Useful for debugging or verifying the sequence contents before execution.
         */
        void dump() const;
        
        /**
         * @brief Executes all actions in the sequence in order, applying the defined delay between each.
         * Iterates through the `actions` list and calls `performAction()` on each entry,
         * pausing for `delayTime` milliseconds between actions.
         * @param keys[VirtualKeyStrokes& keys] - The object for simulating actions.
         * @param testMode[bool] - Enables test mode (default = false).
         * @param [bool] - Enables test mode.
         */
        void performActions(virtual_keys::VirtualKeyStrokes& keys, 
                            bool testMode = false);
    };
    
    /// Maps sequence names to their corresponding list of actions.
    using sequenceList = std::unordered_map<std::string, CompleteSequence>;
	
	/**
	 * Dumps the list of the valid sequences to output stream.
	 * @param out[std::ostream&] - The output stream to print the output to.
	 */
	void printSequenceList(std::ostream& out = std::cout);

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
    SequenceAction createAction(std::string key, std::string value);
    
    /**
     * Runs a sequence based on the name (key) of the sequence.
     * @param sequenceName[const std::String&] - The name of the sequence to run.
     */
    void runSequence(const std::string& sequenceName);
    
    /**
     * Runs a sequence.
     * @param sequence[const CompleteSequence&] - The sequence to run.
     */
    void runSequence(CompleteSequence& sequence);
     
    /**
     * This will load a default front-end. This will continually loop, while asking the
     * use for an input sequence name. When a sequence name is entered. The appropriate
     * sequence will activate, then loop back to the start of the loop.
     */
    void defaultFrontEnd();
    
    /// An instance of VirtualKeyStrokes for calling the virtual key strokes.
    virtual_keys::VirtualKeyStrokes keys;

    /**
     * This will load in the configuration file and load the sequences.
     */
    void loadConfig();
    
    /// The name of the defauilt config file that this class uses.
    std::string defaultSequencesFile{"MIASequences.MIA"};
    
    /**
     * The configuration loader for this app. The configuration for this app uses the
     * constants::ConfigType::RAW_LINES format since it also loads the sequencer fields
     * in which need custom parsing and interpretation.
     */
    config::MIAConfig config;
		
    // Command options for this app.
    CommandOption sequencesFileOpt;  ///< Used for loading a custom sequences file.
    CommandOption testOpt;           ///< Used for enabling test mode.
    CommandOption sequenceNameOpt;   ///< Used for running a particular sequence.
    CommandOption loopModeOpt;       ///< Used for setting the sequence loop mode option.
    CommandOption printSequencesOpt; ///< Used for printing all valid sequences.
	
	/// Determines whether to loop sequences or terminate after they finish.
	bool loopMode{false};
	
	/// Determines whether to print the sequences on application start.
	bool printSequences{false};
    
    /// Name of the option-entered sequence to run. Empty if no sequence option used.
    std::string sequenceName{};
    /// Stores true for testMode functionality.
    bool testMode{false};
    
    /// The complete list of sequences.
    sequenceList sequences;
          
    /**
     * This is a listener, which will (if initialized and set), monitor for a specific
     * user-defined key-press throughout the sequence to stop and/or restart the sequence looping.
     * Since this is a threaded class, this will be constructed outside of the sequences when it
     * is needed and used.
     */
    KeyListenerTask listener{};
    
}; // class MIASequencer
