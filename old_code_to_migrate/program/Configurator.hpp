/**
 * File: Configurator.hpp
 * Author: Antonius Torode
 * Creation Date: 03/01/2021
 * Description: This file controls loading in the configuration file(s)
 */

#include <iostream>
#include <vector>

/**
 * The Configurator class to handle variable declaration and handling of the MIAConfig.MIA file.
 */
class Configurator
{
public:

    /**
     * Main default constructor for the configurator class.
     */
    Configurator();

    /**
     * Main destructor for the configurator class.
     */
    ~Configurator() = default;

    void initialize();

    /**
     * Returns the verboseMode program variable.
     * @return verboseMode[bool]
     */
    static bool getVerboseMode();

    /**
     * File paths related to various program features.
     */
    struct ProgramFilePaths
    {
		// Linux file paths 
        ProgramFilePaths()
        {
			std::string resourcesFolder = findResourcesFolder(__FILE__);
			std::cout << "Detected resources folder: " << resourcesFolder << std::endl;
			MIAConfigFile = resourcesFolder + "/MIAConfig.MIA";
			excuseFilePath = resourcesFolder + "/excuses.MIA";
			inputFilePath = resourcesFolder + "/inputFiles/";
			workoutsFilePath = resourcesFolder + "/inputFiles/workouts.txt";
			workoutOutputFilePath = resourcesFolder + "/outputFiles/workout.txt";
			cryptFilePath = resourcesFolder + "/encryptedFiles/";
			decryptFilePath = resourcesFolder + "/decryptedFiles/";
			sequencesFilePath = resourcesFolder + "/inputFiles/MIASequences.txt";
			shengliDatabaseFilePath = resourcesFolder + "/shengli.MIA";
		};

        /// An enumeration object containing various file types.
        enum filePathType
        {
            UNKNOWNFILEPATH,
            MIACONFIGFILEPATH,
            EXCUSEFILEPATH,
            INPUTFILEPATH,
            WORKOUTSFILEPATH,
            WORKOUTOUTPUTFILEPATH,
            CRYPTFILEPATH,
            DECRYPTFILEPATH,
            SEQUENCESFILEPATH,
            SHENGLIDATABASEFILEPATH
        };

        std::string MIAConfigFile;           ///< File path to the MIAConfig.MIA file.
        std::string excuseFilePath;          ///< File path to the excuses.MIA file.
        std::string inputFilePath;           ///< File path to the input data folder.
        std::string workoutsFilePath;        ///< File path the the exercises.txt input file.
        std::string workoutOutputFilePath;   ///< File path to output workout generated files to.
        std::string cryptFilePath;           ///< File path for input encryption files.
        std::string decryptFilePath;         ///< File path to output decryption files to.
        std::string sequencesFilePath;       ///< File path for input sequencer files.
        std::string shengliDatabaseFilePath; ///< File path for the shengli database file.
    };

    /**
     * Program variables related to WoW features.
     */
    struct WowProgramVariables
    {
        WowProgramVariables() : WoWMailboxSendLetterLocationX(270),
                                WoWMailboxSendLetterLocationY(650),
                                WoWMailboxFirstLetterLocationX(55),
                                WoWMailboxFirstLetterLocationY(265),
                                WoWMailboxLootLetterLocationX(675),
                                WoWMailboxLootLetterLocationY(600),
                                WoWMailboxDeleteLetterLocationX(700),
                                WoWMailboxDeleteLetterLocationY(650),
                                WoWFishBotStartX(725),
                                WoWFishBotStartY(360),
                                WoWFishBotEndX(1230),
                                WoWFishBotEndY(495),
                                WoWFishBotIncrement(40),
                                WoWFishBotNumOfCasts(10000),
                                WoWFishBotDelay(10000)
        {};

        //WoW Mailbox variables.
        int WoWMailboxSendLetterLocationX;
        int WoWMailboxSendLetterLocationY;
        int WoWMailboxFirstLetterLocationX;
        int WoWMailboxFirstLetterLocationY;
        int WoWMailboxLootLetterLocationX;
        int WoWMailboxLootLetterLocationY;
        int WoWMailboxDeleteLetterLocationX;
        int WoWMailboxDeleteLetterLocationY;

        //Fishbot Variables
        int WoWFishBotStartX;
        int WoWFishBotStartY;
        int WoWFishBotEndX;
        int WoWFishBotEndY;
        int WoWFishBotIncrement;
        int WoWFishBotNumOfCasts;
        int WoWFishBotDelay;
    };

    /**
     * Variables used by the sequencer/
     */
    struct SequencerVariables
    {
        SequencerVariables()
        {
            // Set some default values.
            defaultButtonCombination = "1234567T";
            buttonCombinationCoordsX = {};
            buttonCombinationCoordsY = {};
        };

        std::string defaultButtonCombination;
        std::vector<int> buttonCombinationCoordsX;
        std::vector<int> buttonCombinationCoordsY;
    };

    struct DatabaseVariables
    {
        DatabaseVariables() = default;
        std::string username;
        std::string database;
        std::string password;
        std::string hostname;
        unsigned short port{};
    };

    /**
     * Returns the appropriate variables for file paths.
     * @param filePath[std::string&] - Reference to file path to return.
     * @return Returns the string value of the file path requested.
     */
    std::string getFilePath(ProgramFilePaths::filePathType filePath) const;

    /**
     * Returns the appropriate variables for file paths.
     * @param filePath[std::string&] - Reference to file path to return.
     * @return Returns the string value of the file path requested.
     */
    std::string getFilePath(const std::string& filePath) const;


    /**
     * Returns the program version of MIA.
     * @return[std::string] - program version.
     */
    static std::string getMIAVersion();

    /**
     * This function is for loading in the config file.
     * This will initialize all the settings from the MIAConfig.MIA file.
     * @param printSettings
     */
    void initializeSettings(bool printSettings);

    ProgramFilePaths programFilePaths{};
    WowProgramVariables wowProgramVariables{};
    SequencerVariables sequencerVariables{};
    DatabaseVariables databaseVariables{};
	
	/// Initializes and returns private WoW variables.
    int getWoWMailboxDeleteLetterLocation(char coord) const;
    void setWoWMailboxDeleteLetterLocation(char coord, const std::string &value);
    int getWoWMailboxLootLetterLocation(char coord) const;
    void setWoWMailboxLootLetterLocation(char coord, const std::string &value);
    int getWoWMailboxSendLetterLocation(char coord) const;
    void setWoWMailboxSendLetterLocation(char coord, const std::string &value);
    int getWoWMailboxFirstLetterLocation(char coord) const;
    void setWoWMailboxFirstLetterLocation(char coord, const std::string &value);
	
    /**
     * Returns values relating to the WoWFishBotSpace.
     * @param data
     * @return
     */
    int getWoWFishBotSpace(const std::string &data) const;
	
	static std::string findResourcesFolder(const std::string& currentDirectory);
	
protected:

private:

    /// Program version of MIA.
    static std::string VERSION;

    /**
     * Enables or disables verbose mode.
     * @param value[std::string] - "1" or "true" for enable, otherwise disabled.
     */
    static void setVerboseMode(std::string value);

    /**
     * Enables or disables verbose mode.
     * @param value[std::string] - true for enable, otherwise disabled.
     */
    static void setVerboseMode(bool value);

    /**
     * Variables related to main program functions.
     */
    struct ProgramVariables
    {
        static bool verboseMode;            ///< Enables extended program output.
		static std::string MIA_VERSION;
    };


    /**
     * Set's the variable to a value.
     * @param variable[std::string&] The string representing the variable name.
     * @param value[std::string&] The value representing the variable value.
     */
    void setMIAVariables(const std::string &variable, const std::string &value);

    /**
     * Returns true if a string is a reference to a valid filepath.
     * @param variable[std::string&] - Reference to fileType string.
     * @return Returns true if the file type is valid.
     */
    static bool variableIsFilePath(const std::string &variable);

    /**
     * Converts a string file path name to a ProgramFilePaths::filePathType value.
     * @param filePath[std::string&] - String reference to a file path type.
     * @return [ProgramFilePaths::filePathTypes] the corresponding filePathType
     */
    static ProgramFilePaths::filePathType stringFilePathToEnumFilePath(const std::string& filePath);

    /**
     * Converts a ProgramFilePaths::filePathType file path name to a string value.
     * @param filePath[std::string&] - String reference to a file path type.
     * @return [ProgramFilePaths::filePathTypes] the corresponding filePathType
     */
    static std::string enumFilePathToStringFilePath(ProgramFilePaths::filePathType filePath);

    /**
     * Determines if a ProgramFilePaths::filePathTypes is valid.
     * @param variable[ProgramFilePaths::filePathTypes] The file type to check.
     * @return [pbool] Returns false if unknown file path type.
     */
    static bool variableIsFilePath(ProgramFilePaths::filePathType variable);

    /**
     * Sets the appropriate variables for file paths.
     * @param filePath[std::string&] - Reference to file path to set.
     * @param input[std::string&] - Refernce to file path value used to set file path.
     */
    void setFilePath(const std::string& filePath, const std::string& input);

    /**
     * Sets the appropriate variables for file paths.
     * @param filePath[std::string&] - Reference to file path to set.
     * @param input[std::string&] - Refernce to file path value used to set file path.
     */
    void setFilePath(ProgramFilePaths::filePathType filePath, const std::string& input);

    /**
     * Sets values relating to the WoWFishBotSpace.
     * @param data
     * @param value
     */
    void setWoWFishBotSpace(const std::string &data, const std::string &value);


    void setDefaultButtonCombination(const std::string &input);
    std::string getDefaultButtonCombination() const;

    /// Methods for setting database variables.
    void setUsername(const std::string &input);
    void setDatabase(const std::string &input);
    void setPassword(const std::string &input);
    void setHostname(const std::string &input);
    void setPort(const std::string &input);
};