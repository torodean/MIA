/**
 * @file Maple.hpp
 * @author Antonius Torode
 * @date 08/16/2026
 * @brief This file defines the main API for the Monetary Automation Projection and Learning 
 * Extension (MAPLE) application. 
 */
#pragma once

#include <string>
#include <vector>

// Used for the configuration.
#include "MIAConfig.hpp"
// Used for common app setup.
#include "MIAApplication.hpp"
// Storage for tax calculation values.
#include "TaxConstants.hpp"
// Storage for the income
#include "MoneyHandler.hpp"

namespace maple
{
    /// The name of the defauilt config file that this class uses.
    const std::string defaultConfigFile{"Maple.MIA"};
        
    /**
     * This enum defines the various operations/tasks that are available to the Maple app.
     * When adding new operations, the operationToDesc() method will need updated along
     * with this enum.
     */
    enum MapleOperations : uint8_t
    {
        PrintOpList = 0,      ///< Prints the list of operations.
        testOption = 1,       ///< Reserved for testing to develop new features.
        TaxCalculation = 2,   ///< Calculations annual taxes owed.
        OperationCount = 3    ///< The total number of operations in this enum. KEEP LAST!
    }; // enum MapleOperations
    
    /**
     * Prints the description of a valid maple operation. This will print the index of
     * the operation, the operation name, and a short description of the operation.
     * @param operation The operation to get information about.
     * @return A String representation of the operation information. 
     */
    std::string operationToDesc(MapleOperations operation);
    
    /**
     * @brief Formats a description of an operation based on various inputs.
     *
     * The string constructed by this method is of the following form:
     *     "index) name - description"
     *
     * @param operation The operation to format - needed for the index.
     * @param name The name of the operation.
     * @param desc The descroption of the operation.
     * @return A string formatted description.
     */
    std::string formatDesc(MapleOperations operation,
                           const std::string& name,
                           const std::string& desc);
    
    /**
     * @class Maple
     * @brief The Main application class for the Maple app.
     */
    class Maple : public MIAApplication
    {
    public:

        /**
         * The main constructor of the Maple class. This will construct the command options.
         */
        Maple();

        /// Default destructor.
        ~Maple() = default;
        
        /**
         * This will initialize this class by parsing command line arguments for 
         * configuration options and then loading the config file if provided.
         *
         * @param argc Number of command line arguments.
         * @param argv Array of command line argument strings.
         */
        void initialize(int argc, char* argv[]) override;
        
        /** 
         * @brief Runs the Maple application.
         */
        int run() override;

    protected:

        /**
         * Prints help info for optional config file.
         */
        virtual void printHelp() const;

    private:
    
        /**
         * This will load a default front-end. This will continually loop, while asking the
         * use for an input option. When a valid input is entered. The appropriate
         * feature will be performed, then loop back to the start of the loop.
         */
        void defaultFrontEnd();
        
        /// Prints a list of the valid operations programmed into Maple.
        void printOperationsList();
        
        /**
         * @brief Maps an operation to the correct function/methods.
         * @param operation The index of the operation to perform.
         * @return true if the operation is valid. false otherwise.
         */
        bool runOperation(MapleOperations operation);
    
        /// Just a temporary method for testing while developing the app.
        void test();
       
        /**
         * TODO.
         */
        bool loadConfig();
        
        /**
         * The configuration loader for this app.
         */
        config::MIAConfig config;
            
        // Command options for this app.
        CommandOption configOpt;   ///< The config file to use for this app.      
        CommandOption testOpt;     ///< Used for enabling test mode.
        
        /// Stores true for testMode functionality.
        bool testMode{false};
        
        /// Storage for the tax constants.
        TaxRateConstants federalTaxConstants;
        TaxRateConstants stateTaxConstants;
        /// Storage for the income.
        MoneyHandler income;
        /// Storage for the expenses.
        MoneyHandler expenses;
        
    }; // class Maple
} // namespace maple
