/**
 * File: MIAWorkout.hpp
 * Author: Antonius Torode
 * Created on: 03/07/2021
 * Description: A program used to generate workouts.
 */
#pragma once

// Used for infinite().
#include <limits>

// Used for the configuration.
#include "MIAConfig.hpp"
// Used for common app setup.
#include "MIAApplication.hpp"
// Used for command options.
#include "CommandOption.hpp"

class MIAWorkout : public MIAApplication
{
public:

    /**
     * The main constructor of the MIAWorkout class. This will construct the command options.
     */
    MIAWorkout();

    /// Default destructor.
    ~MIAWorkout() = default;
    
    /**
     * This will initialize this class by parsing command line arguments for configuration options
     * and then loading the config file if provided.
     *
     * @param argc Number of command line arguments.
     * @param argv Array of command line argument strings.
     */
    void initialize(int argc, char* argv[]) override;
    
    /// Runs the MIAWorkout.
    int run() override;

protected:

    /**
     * Prints help info for optional config file.
     */
    virtual void printHelp() const;

private:

    /**
     * Returns a workout generated based on the difficulty.
     */
    void generateWorkout();
    
    /// The configuration loader for this app.
    config::MIAConfig config;

    // Command options for this app.
    CommandOption configFileOpt;
    CommandOption outputFileOpt;
    CommandOption difficultyOpt;
    CommandOption weeklyOpt;
    
    /// The output file path.
    std::string workoutOutputFilePath{""};
    
    /// The difficulty
    double difficulty{1.0};
    /// Whether or not to print an entire week of workouts.
    bool weekly{false};
    /// The toughness of the workouts.
    double toughness{0.1};
    /// The min and max number of exercises.
    double minNumOfExercises{3.0}, maxNumOfExercises{std::numeric_limits<double>::infinity()};
    /// The min and max number of exercises.
    double minNumOfSets{3.0}, maxNumOfSets{std::numeric_limits<double>::infinity()};
    
    /// The name of the defauilt config file that this class uses.
    std::string defaultConfigFile{"MIAWorkout.MIA"};
    
    /// Load the configuration variables.
    void loadConfig();

    /**
     * Convert input file line to proper format (doubles.
     * @param line
     * @return
     */
    static double convertWorkoutWeight(const std::string& line);

    /**
     * Returns the maximumNumberOfXXX based on a difficulty.
     * Custom formula chosen for desired workout curves.
     * @param min
     * @param max
     * @param difficulty
     * @return
     */
    static double maxNumModifier(double min, double max, double difficulty);

    /**
     * Returns the manimumNumberOfXXX based on a difficulty.
     * Custom formula chosen for desired workout curves.
     * @param min
     * @param max
     * @param difficulty
     * @return
     */
    static double minNumModifier(double min, double max, double difficulty);
};
