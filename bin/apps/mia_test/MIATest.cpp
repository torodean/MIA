/**
 * @file MIATest.cpp
 * @author Antonius Torode
 * @date 09/03/2026
 * @brief Implements any test features.
 */

#include <iostream>
#include <string>
#include <vector>

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
     * object goes out of scope. A module which fails to load throws from the
     * constructor, so a constructed module is always usable.
     */
    PythonModule module("MIATest", __FILE__);

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
     * The variadic call() converts each argument by its C++ type, so integer
     * literals, doubles, and string literals all work without casts or
     * suffixes, and mixed signatures need no dedicated overload.
     */
    report("main()", module.call("main"));
    report("add(2, 3)", module.call("add", 2, 3));
    report("multiply(2.5, 4.0)", module.call("multiply", 2.5, 4.0));
    report("greet('user')", module.call("greet", "user"));
    report("describe('user', 30)", module.call("describe", "user", 30));
    report("repeat('word', 3)", module.call("repeat", "word", 3));
    report("printSum(10, 5)", module.call("printSum", 10, 5));
    report("sumInts({1, 2, 3, 4, 5})", module.call("sumInts", std::vector<int>{1, 2, 3, 4, 5}));
    report("sumDoubles({1.5, 2.5, 3.5})", module.call("sumDoubles", std::vector<double>{1.5, 2.5, 3.5}));

    // One call to a missing method to verify the error path reports cleanly.
    report("missingMethod()", module.call("missingMethod"));

    std::cout << "Tests finished!" << std::endl;
    return allPassed ? constants::SUCCESS : constants::FAILURE;
}
