/**
 * @file MIATest.cpp
 * @author Antonius Torode
 * @date 09/03/2026
 * @brief Implements any test features.
 */

#include <iostream>
#include <string>

// The associated header file.
#include "MIATest.hpp"
// Used for the Python module wrapper being tested.
#include "PythonModule.hpp"


MIATest::MIATest()                      
{ };


void MIATest::initialize(int argc, char* argv[])
{
	// TODO
}

int MIATest::run()
{
    /*
     * Constructing the module is the only setup needed: the wrapper starts
     * the embedded interpreter on first use and shuts it down when the module
     * object goes out of scope.
     */
    PythonModule module("MIATest");

    if (!module.isLoaded())
    {
        std::cerr << "Failed to load module 'MIATest'." << std::endl;
        return constants::FAILURE;
    }

    // Prints the result of one call, reporting the error path instead of the
    // value when the call failed.
    bool allPassed = true;
    auto report = [&allPassed](const std::string& label, const PythonResult& result)
    {
        if (!result.isValid())
        {
            allPassed = false;
            std::cout << label << " failed: " << result.getError() << std::endl;
            return;
        }

        switch (result.getType())
        {
            case PythonResult::Type::Integer:
                std::cout << label << " -> " << result.asInt() << std::endl;
                break;
            case PythonResult::Type::Double:
                std::cout << label << " -> " << result.asDouble() << std::endl;
                break;
            case PythonResult::Type::String:
                std::cout << label << " -> " << result.asString() << std::endl;
                break;
            case PythonResult::Type::Void:
            default:
                std::cout << label << " -> (void)" << std::endl;
                break;
        }
    };

    // Exercise hasMethod on one present and one missing method.
    std::cout << "hasMethod('add'): "
              << (module.hasMethod("add") ? "true" : "false") << std::endl;
    std::cout << "hasMethod('missingMethod'): "
              << (module.hasMethod("missingMethod") ? "true" : "false") << std::endl;

    /*
     * Integer literals use the long suffix so overload resolution picks
     * the integer overload; plain int literals would be ambiguous between
     * the (long, long) and (double, double) overloads.
     */
    report("main()", module.call("main"));
    report("add(2, 3)", module.call("add", 2L, 3L));
    report("multiply(2.5, 4.0)", module.call("multiply", 2.5, 4.0));
    report("greet('user')", module.call("greet", std::string("user")));
    report("describe('user', 30)", module.call("describe", std::string("user"), 30L));
    report("repeat('word', 3)", module.call("repeat", std::string("word"), 3L));
    report("printSum(10, 5)", module.call("printSum", 10L, 5L));

    // One call to a missing method to verify the error path reports cleanly.
    report("missingMethod()", module.call("missingMethod"));

    std::cout << "Tests finished!" << std::endl;
    return allPassed ? constants::SUCCESS : constants::FAILURE;
}
