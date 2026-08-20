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
// Storage for the expense
#include "MonthlyExpenses.hpp"
// Storage for the income
#include "Income.hpp"

namespace maple
{
    /// The name of the defauilt config file that this class uses.
    const std::string defaultConfigFile{"Maple.MIA"};
        
    /**
     * This enum defines the various commands/tasks that are available to the Maple app.
     */
    enum MapleCommands
    {
        TaxCalculation,   ///< Calculations annual taxes owed. 
    }; // enum MapleCommands
    
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
         * TODO
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
        TaxRateConstants taxConstants;
        /// Storage for the income.
        Income income;
        /// Storage for the expenses.
        MonthlyExpenses expenses;
        
    }; // class Maple
} // namespace maple
