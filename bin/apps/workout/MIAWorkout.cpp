/**
 * @file MIAWorkout.cpp
 * @author Antonius Torode
 * @date 03/07/2021
 * Description:
 */

#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

// The associated header file.
#include "MIAWorkout.hpp"
#include "StringUtils.hpp"
#include "MathUtils.hpp"

// Used for error handling.
#include "Error.hpp"
#include "Paths.hpp"

// Used for the key-value pair
#include "Constants.hpp"

using std::string;
using std::cout;
using std::endl;
using std::vector;


MIAWorkout::MIAWorkout() : 
    config(defaultConfigFile, constants::ConfigType::KEY_VALUE),
    configFileOpt("-c", "--config", "Specify a config file to use (default = " +
                                paths::getDefaultConfigDirToUse() + "/MIAWorkout.MIA)",
                                CommandOption::commandOptionType::STRING_OPTION),
    outputFileOpt("-o", "--output", "Specify the output file path for workouts (defaults"
                                    " to config file value).",
                                CommandOption::commandOptionType::STRING_OPTION),
    difficultyOpt("-d", "--difficulty", "Specify a difficulty to use (default = 1.0).",
                                CommandOption::commandOptionType::DOUBLE_OPTION),
    weeklyOpt("-w", "--weekly", "Generate workouts for an entire week.",
                                CommandOption::commandOptionType::BOOL_OPTION)
                                    
{ };


void MIAWorkout::initialize(int argc, char* argv[])
{
    try
    {  
        MIAApplication::initialize(argc, argv);

        // Set the values from the command line arguments.
        difficultyOpt.getOptionVal<double>(argc, argv, difficulty);
        weeklyOpt.getOptionVal<bool>(argc, argv, weekly);
        outputFileOpt.getOptionVal<std::string>(argc, argv, workoutOutputFilePath);
        
        std::string configFile = defaultConfigFile;
        configFileOpt.getOptionVal<std::string>(argc, argv, configFile);
        config.setConfigFileName(configFile, constants::ConfigType::KEY_VALUE); // handles config.initialize().
    }
    catch (const error::MIAException& ex)
    {
        std::cerr << "Error during MIAWorkout::initialize: " << ex.what() << std::endl;
    }
    
    loadConfig();
}

void MIAWorkout::loadConfig()
{
    try
    {
        if (workoutOutputFilePath != "") // Otherwise it was set via a command option.
            workoutOutputFilePath = config.getString("workoutOutputFilePath");
        
        // Get config values.
        toughness = config.getDouble("toughness");
        minNumOfExercises = config.getDouble("minNumOfExercises");
        maxNumOfExercises = config.getDouble("maxNumOfExercises");
        minNumOfSets = config.getDouble("minNumOfSets");
        maxNumOfSets = config.getDouble("maxNumOfSets");
    }
    catch (error::MIAException& ex)
    {
        std::cerr << "Error loading in configuration file value(s): " 
        << ex.what() 
        << "Continuing with default values."
        << std::endl;
    }
    
    if (getVerboseMode())
        config.dumpConfigMap();
}

void MIAWorkout::printHelp() const
{
    MIAApplication::printHelp();
    
    cout << "MIAWorkout specific options:" << endl
         << configFileOpt.getHelp() << endl
         << outputFileOpt.getHelp() << endl
         << difficultyOpt.getHelp() << endl
         << weeklyOpt.getHelp() << endl
         << endl << endl;
    cout << "Assuming default values, difficulties range from 0-100." << endl;
    cout << "   1-10 (VERY EASY), 11-24 (EASY), 25-39 (NORMAL)" << endl;
    cout << "   39-54 (HARD), 54-74 (VERY HARD), 75+ (INSANE)" << endl;
}

double MIAWorkout::convertWorkoutWeight(const string& line)
{
    return stod(line);
}


double MIAWorkout::maxNumModifier(double min, double max, double difficulty)
{
    return (max/(10.0*pow(10.0,(1.0/3.0))) - min/(10.0*pow(10.0,(1.0/3.0))))*pow(difficulty,(2.0/3.0))+min;
}

double MIAWorkout::minNumModifier(double min, double max, double difficulty)
{

    //adjusts the min compared to max.
    //Set such that new curves are similar to old workout generation functions.
    double minModifier = 1.9;
    

    return (-((-max + min*minModifier)/(10.0*pow(10.0,(1.0/3.0))*minModifier)))*pow(difficulty,(2.0/3.0)) + min;
}

void MIAWorkout::generateWorkout()
{
    // Int to keep track of whether we need to generate a weekly workout or not.
    int timesToGenerate = 1;
    std::ofstream out;

    if(weekly)
    {
        string outputFile = workoutOutputFilePath;
        outputFile.erase(outputFile.end()-4, outputFile.end());
        outputFile += " ";
        outputFile += __DATE__;
        outputFile += ".txt";

        cout << "...Weekly workout generation loaded. " << endl;
        cout << "...Workout will be output to '" << outputFile << "'" << endl;
        cout << "..." << endl;

        string outputFileText = "//============================================================================\n"
                                "// Name        : workout.txt                                                  \n"
                                "// Author      : MIA                                                          \n"
                                "// Created on  : " + StringUtils::today() + "                                       \n"
                                "// Description : A daily generated workout via the MIA program (for one week).\n"
                                "// Difficulty  : " + std::to_string(difficulty) + "                           \n"
                                "//============================================================================\n\n";

        timesToGenerate = 7; //generate 7 workouts
        out.open(outputFile);
        out << outputFileText;
    }
    
    // All the exercises are stored in the config file.
    std::vector<constants::KeyValuePair> exercisesList = config.getAllConfigPairs();

    // Creates and stores workouts file/program variables.
    vector<string> workoutName;
    vector<double> workoutWeight;
    vector<string> workoutUnit;
    int maxNumOfExercises = 0; // Counter for number of exercises. Doubles as a counter.
    
    // Sets the workout file variables and the weighted value.
    for(const auto& exercise : exercisesList)
    {
        if (!exercise.second.empty() &&                              // skip empty lines
            StringUtils::hasExactlyOneOfADelimiter(exercise.second, ';'))  // Make sure it's an exercise line.
        {
            workoutName.push_back(exercise.first);
            workoutWeight.push_back( convertWorkoutWeight( StringUtils::getBeforeChar(exercise.second, ';') ) );
            workoutUnit.push_back(StringUtils::getAfterChar(exercise.second, ';') );

            if(getVerboseMode())
            {
                cout << "...Printing values set. " << endl;
                cout << workoutName[maxNumOfExercises] << ": ";
                cout << workoutWeight[maxNumOfExercises] << " ";
                cout << workoutUnit[maxNumOfExercises] << endl;
            }
            maxNumOfExercises++; // Increment for new exercise found.
        }
    }

    cout << "...Running workout generation. " << endl;

    int counter = 1; //For changing random integers between calls.

    // Loops over the generation code however many times needed.
    while(timesToGenerate > 0)
    {

        if(!weekly)
        {
            cout << endl;
        } 
        else if(timesToGenerate == 7)
        {
            out << "##############\n"
                   "### SUNDAY ###\n"
                   "##############" << endl << endl;
        } 
        else if(timesToGenerate == 6)
        {
            out << "##############\n"
                   "### MONDAY ###\n"
                   "##############" << endl << endl;
        } 
        else if(timesToGenerate == 5)
        {
            out << "###############\n"
                   "### TUESDAY ###\n"
                   "###############" << endl << endl;
        } 
        else if(timesToGenerate == 4)
        {
            out << "#################\n"
                   "### WEDNESDAY ###\n"
                   "#################" << endl << endl;
        } 
        else if(timesToGenerate == 3)
        {
            out << "################\n"
                   "### THURSDAY ###\n"
                   "################" << endl << endl;
        } 
        else if(timesToGenerate == 2)
        {
            out << "##############\n"
                   "### FRIDAY ###\n"
                   "##############" << endl << endl;
        } 
        else if(timesToGenerate == 1)
        {
            out << "################\n"
                   "### SATURDAY ###\n"
                   "################" << endl << endl;
        }

        //Calculates workout routine
        double maxNumOfExercisesModifier = maxNumModifier(minNumOfExercises, static_cast<double>(maxNumOfExercises), difficulty);
        double minNumOfExercisesModifier = minNumModifier(minNumOfExercises, static_cast<double>(maxNumOfExercises), difficulty);
        double maxNumOfSetsModifier = maxNumModifier(minNumOfSets, maxNumOfSets, difficulty);
        double minNumOfSetsModifier = minNumModifier(minNumOfSets, maxNumOfSets, difficulty);

        int numOfWorkouts;
        counter++;
        int numOfSets = math::randomInt((int)minNumOfSetsModifier, (int)maxNumOfSetsModifier, counter, true);

        if (getVerboseMode())
        {
            std::cout << "maxNumOfExercisesModifier: " << maxNumOfExercisesModifier << std::endl;
            std::cout << "minNumOfExercisesModifier: " << minNumOfExercisesModifier << std::endl;
            std::cout << "maxNumOfSetsModifier: " << maxNumOfSetsModifier << std::endl;
            std::cout << "minNumOfSetsModifier: " << minNumOfSetsModifier << std::endl;
            std::cout << "numOfSets: " << numOfSets << std::endl;
        }
            
        if(weekly)
        {
            out << "...Number of sets: " << numOfSets << endl << endl;
        } 
        else 
        {
            cout << "...Number of sets: " << numOfSets << endl;
            cout << endl;
        }

        // declares some needed variables.
        vector<bool> workoutChosen;
        workoutChosen.reserve(maxNumOfExercises);
        for(int i=0; i<maxNumOfExercises; i++)
        {
            workoutChosen.push_back(false);
        }
        counter++;
        int randNum = math::randomInt(0, maxNumOfExercises, counter, true);
        int set = 1;
        double repsMin, repsMax, repsModifier;

        // Loops over the stuff and creates a random workout for each set.
        while (numOfSets > 0)
        {
            counter++;
            if(weekly)
            { //prints results.
                out << "...Workout for set " << set << "." << endl << endl;
            } else {
                cout << "...Workout for set " << set << "." << endl;
                cout << endl;
            }


            for (int i=0; i<maxNumOfExercises; i++)
            {
                workoutChosen[i] = false;
            }

            // randomizes the number of workouts per set.
            int randCounter = randNum;
            numOfWorkouts = math::randomInt((int)minNumOfExercisesModifier, (int)maxNumOfExercisesModifier, randCounter, true);

            // determines which workouts to use.
            for(int i=0;i<numOfWorkouts; i++)
            {
                randNum = math::randomInt(0, maxNumOfExercises, randCounter, true);
                while(workoutChosen[randNum])
                {
                    randNum = math::randomInt(0, maxNumOfExercises, randCounter, true);
                    randCounter++;
                }
                workoutChosen[randNum] = true;
            }

            // Determines values for each workout and prints the results.
            for (int i=0; i<maxNumOfExercises; i++)
            {
                randCounter++;
                if(workoutChosen[i]){
                    repsMin = (toughness*difficulty*workoutWeight[i]-workoutWeight[i])/2+workoutWeight[i];
                    repsMax = toughness*difficulty*workoutWeight[i];
                    if(workoutName[i] != "running"){
                        repsModifier = math::randomInt((int)repsMin, (int)repsMax, randCounter, true);
                        if(weekly)
                        { //prints results.
                            out << workoutName[i] << ": " << (int)repsModifier+1 << " " << workoutUnit[i] << endl;
                        } 
                        else 
                        {
                            cout << workoutName[i] << ": " << (int)repsModifier+1 << " " << workoutUnit[i] << endl;
                        }
                    } 
                    else 
                    {
                        repsModifier = math::randomInt((int)(repsMin*100.0), (int)(repsMax*100.0), randCounter, true);
                        repsModifier /= 100;
                        if(weekly)
                        { //prints results.
                            out << workoutName[i] << ": " << repsModifier << " " << workoutUnit[i] << endl;
                        } 
                        else 
                        {
                            cout << workoutName[i] << ": " << repsModifier << " " << workoutUnit[i] << endl;
                        }
                    }
                }
            }
            
            set++;
            numOfSets--;
            
            if(weekly) //prints results.
                out << endl;
            else
                cout << endl;
                
        }
        if(difficulty == 0.0)
        {
            if(weekly)
            { //prints results.
                out << "rest: 45 minutes" << endl;
            } 
            else 
            {
                cout << "rest: 45 minutes" << endl;
            }
        }
        if (weekly) //prints results.
            out << endl;
        else
            cout << endl;
            
        timesToGenerate--;
    }
    cout << "...Workout generation completed." << endl;
} // void MIAWorkout::generateWorkout(..)


int MIAWorkout::run()
{
    cout << "...Loading MIA workout generator. " << endl;
    cout << "..." << endl;

    generateWorkout();
    return constants::SUCCESS;
}
