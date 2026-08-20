/**
 * @file MIASequencer.hpp
 * @author Antonius Torode
 * @date 12/26/2019
 *
 * A sequencer for processing and executing MIASequences from file.
 * This component supports loading, parsing, and triggering predefined
 * input sequences including coordinates and actions.
 */
#pragma once

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
#include "Sequences.hpp"

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
     * The main constructor of the MIASequencer class. This will construct the command options.
     */
    MIASequencer();

    /// Default destructor.
    ~MIASequencer() = default;
    
    /**
     * @brief This will initialize this class by parsing command line arguments for 
     * configuration options and then loading the config file if provided.
     *
     * @param argc Number of command line arguments.
     * @param argv Array of command line argument strings.
     */
    void initialize(int argc, char* argv[]) override;
    
    /// Runs the MIASequencer application.
    int run() override;
    
    /**
     * @brief Dumps the list of the valid sequences to output stream.
     * @param out The output stream to print the output to.
     */
    void printSequenceList(std::ostream& out = std::cout);

protected:

    /**
     * @brief Prints help info for optional config file.
     */
    virtual void printHelp() const;

private:
    
    /**
     * @brief This will create a SequenceAction from a key-value pair.
     * @param key The key defining the action type.
     * @param value The value defining the data needed to perform the action.
     * @return Returns the constructed SequenceAction.  
     */
    sequences::SequenceAction createAction(std::string key, std::string value);
    
    /**
     * @brief Runs a sequence based on the name (key) of the sequence.
     * @param sequenceName The name of the sequence to run.
     */
    void runSequence(const std::string& sequenceName);
    
    /**
     * @brief Runs a sequence.
     * @param sequence The sequence to run.
     */
    void runSequence(sequences::CompleteSequence& sequence);
     
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
    sequences::sequenceList sequences;
          
    /**
     * This is a listener, which will (if initialized and set), monitor for a 
     * specific user-defined key-press throughout the sequence to stop and/or 
     * restart the sequence looping. Since this is a threaded class, this will 
     * be constructed outside of the sequences when it is needed and used.
     */
    KeyListenerTask listener{};
    
}; // class MIASequencer
