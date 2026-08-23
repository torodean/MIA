/**
 * @file Maple.cpp
 * @author Antonius Torode
 * @date 08/16/2026
 * @brief The main implementation file for the Maple application 
 */

// The pairing header file which defines the class methods for Maple.
#include "Maple.hpp"

#include <iostream>
#include <sstream>

// Used for error handling and configuration.
#include "Paths.hpp"
#include "MIAException.hpp"
// Used for the ConfigType
#include "Constants.hpp"
#include "StringUtils.hpp"


namespace maple
{
    Maple::Maple() :
        configOpt("-c", "--configFile", "Specify the config file to use (default = " +
                  paths::getDefaultConfigDirToUse() + "/" + defaultConfigFile,
                  CommandOption::commandOptionType::STRING_OPTION),
        testOpt("-t", "--test", 
                "Enables test mode. This mode will allow various testing during development.",
                CommandOption::commandOptionType::BOOL_OPTION)
    {}
    

    void Maple::initialize(int argc, char* argv[])
    {
        try
        {
            MIAApplication::initialize(argc, argv);
            
            // Set the values from the command line arguments.
            testOpt.getOptionVal<bool>(argc, argv, testMode);
            
            // Set and load the config file.
            std::string configFile = defaultConfigFile;
            configOpt.getOptionVal<std::string>(argc, argv, configFile);
            // handles config.initialize().
            config.setConfigFileName(configFile, constants::ConfigType::KEY_VALUE);
        }
        catch (const error::MIAException& ex)
        {
            std::cerr << "Error during Maple::initialize: " << ex.what() << std::endl;
        }
        
        // TODO - handle error case. Currently loadConfig always returns true.
        (void)loadConfig();
    }
    
    
    void Maple::test()
    {
        std::cout << "Federal Tax Constants: {" << federalTaxConstants << "}" << std::endl;
        std::cout << "----------------------" << std::endl;
        std::cout << "State Tax Constants: {" << stateTaxConstants << "}" << std::endl;
        std::cout << "----------------------" << std::endl;
        std::cout << "Income: {" << income << "}" << std::endl;
        std::cout << "----------------------" << std::endl;
        std::cout << "Expenses: {" << expenses << "}" << std::endl;
    }
    

    int Maple::run()
    {        
        defaultFrontEnd();        
        return constants::ReturnCode::SUCCESS;
    }
    
    
    void Maple::defaultFrontEnd()
    {
        std::string input;
        
        std::cout << "Valid operations are as follows:" << std::endl;
        printOperationsList();
        
        // Loop over the default interface.
        while (true) 
        {
            std::cout << "Enter the index of an operation to perform: ";
            std::getline(std::cin, input);

            if (input.empty()) 
                continue;
                
            // Perform the operation.
            if (StringUtils::is_digits(input) &&   // Ensure an int was entered.
                !runOperation(static_cast<MapleOperations>(std::stoi(input))) )  // Attempt to run the operation.
            { // Failure case.
                std::cout << "Invalid index entered: " << input << std::endl;
            }
        }
    }
    
    
    std::string operationToDesc(MapleOperations operation)
    {
        std::string output;
        switch(operation)
        {
            case PrintOpList:
                output = formatDesc(operation, 
                                    "List Operations", 
                                    "Prints this list of valid operations.");
                break;
            case testOption:
                output = formatDesc(operation, 
                                    "Test Operation ", 
                                    "Performs test-specific features (for development).");
                break;
            case TaxCalculation:
                output = formatDesc(operation, 
                                    "Calculate Taxes", 
                                    "Calculates various tax-related information.");
                break;
            default:
                output = "Invalid Operation";
                break;
        }
        return output;
    }
    
    
    std::string formatDesc(MapleOperations operation,
                       const std::string& name,
                       const std::string& desc)
    {
        std::stringstream stream;
        stream << static_cast<unsigned>(operation)
               << ") " << name
               << " - " << desc;
       return stream.str();
    }
    
    void Maple::printOperationsList()
    {
        std::cout << "--------------------------------------------" << std::endl;
        for (uint8_t i=0; i<OperationCount; i++)
            std::cout << operationToDesc(static_cast<MapleOperations>(i)) << std::endl;
        std::cout << "--------------------------------------------" << std::endl;
    }
    
    
    bool Maple::runOperation(MapleOperations operation)
    {
        // Storage for operation containers.
        
        switch(operation)
        {
            case PrintOpList: 
                printOperationsList(); 
                break;
            case testOption:  
                test();
                break;
            case TaxCalculation:
                // Only recalculate if this hasn't been done yet.
                if (!taxOperationReturns.initialized)
                    taxOperationReturns = calculateTaxesOperation(income,
                                                                  expenses,
                                                                  federalTaxConstants, 
                                                                  stateTaxConstants,
                                                                  miscTaxValues);
                printTaxOperationReturns(taxOperationReturns);            
                break;
            default: 
                return false;
        }
        return true;
    }
    

    void Maple::printHelp() const
    {
        MIAApplication::printHelp();
    
        // This is a dump of the help messages used by the various command options.
        std::cout << "Maple specific options:" << std::endl
                  << configOpt.getHelp() << std::endl
                  << testOpt.getHelp() << std::endl
                  << std::endl;
    }


    bool Maple::loadConfig()
    {
        bool printWarnings = getVerboseMode();
    
        federalTaxConstants = createTaxRateConstantsFromConfig(config, "_federal", printWarnings);
        stateTaxConstants = createTaxRateConstantsFromConfig(config, "_state", printWarnings);
        income = createMoneyHandlerFromConfig(config, "income", printWarnings);
        expenses = createMoneyHandlerFromConfig(config, "expense", printWarnings);
        miscTaxValues = createMapleMiscTaxFromConfig(config, "tax", printWarnings);
        
        return true;
    }
} // namespace Maple
