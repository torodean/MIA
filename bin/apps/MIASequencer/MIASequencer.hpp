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

// Used for key strokes.
#include "VirtualKeyStrokes.hpp"


/**
 * A template app.
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
		
    // Command options for this app.
    CommandOption sequencesFileOpt;  ///< Used for loading a custom sequences file.
    CommandOption testOption;        ///< Used for enabling test mode.
    
    /// The name of the defauilt config file that this class uses.
    std::string defaultSequencesFile{"MIASequences.MIA"};
    
}; // class MIASequencer
