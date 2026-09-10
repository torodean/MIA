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
// Used for testing the PythonPlotter.
#include "PythonPlotter.hpp"


MIATest::MIATest()                      
{ };


/*
 * This method currently does what the base class does... It is only here
 * so that I do not need to re-add it when adding test options that need parsed.
 */
void MIATest::initialize(int argc, char* argv[])
{
    try
    {    
        MIAApplication::initialize(argc, argv);
    }
    catch (const error::MIAException& ex)
    {
        std::cerr << "Error during MIATest::initialize: " << ex.what() << std::endl;
    }
}


int testPythonModule()
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


int testPythonPlotter(bool verboseMode)
{
    // Sequence to test plotting with.
    std::vector<int> x = {0,1,2,3,4,5,6,7};
    std::vector<int> y = {0,1,3,6,10,15,21,28};
    
    python_plotting::PythonPlotter plotter;
    if (verboseMode)
        plotter.setVerboseOutput(true);
    plotter.setLabels("MIATest Plot", "X-Values", "Y-Values");
    return plotter.plot(x,y);        
}


/**
 * @brief Tests the PythonPlotter multi-line plotting method.
 * @return The result of the plot operation.
 */
int testPythonPlotterMultiLine(bool verboseMode)
{
    // Sequence to test plotting with.
    std::vector<int> x = {0,1,2,3,4,5,6,7};

    python_plotting::LinesToPlot<int> data = {
        {
            {}, // Empty xValues: use the shared x-axis.
            {0,1,3,6,10,15,21,28},
            python_plotting::LineStyle::solid,
            4.0,
            python_plotting::Color::blue,
            "Triangular numbers"
        },
        {
            {}, // Empty xValues: use the shared x-axis.
            {0,1,2,4,8,16,32,64},
            python_plotting::LineStyle::dashed,
            1.0,
            python_plotting::Color::red,
            "Powers of two"
        }
    };

    python_plotting::PythonPlotter plotter;
    if (verboseMode)
        plotter.setVerboseOutput(true);
    plotter.setLabels("MIATest Plot", "X-Values", "Y-Values");
    plotter.enableLegend(true);
    plotter.setShowGrid(true);
    plotter.setFigureSize(10.0, 6.0);

    return plotter.plot(x, data);
}


/**
 * @brief Tests the PythonPlotter per-line x-axis and the legend label rules.
 *
 * Each line carries its own xValues, so the shared x-axis is unused. One line
 * has an empty label to verify it is left out of the legend.
 * @return The result of the plot operation.
 */
int testPythonPlotterPerLineX(bool verboseMode)
{
    python_plotting::LinesToPlot<int> data = {
        {
            {0,1,2,4,8,16,32,64}, // This line's own x-axis values.
            {0,1,3,6,10,15,21,28},
            python_plotting::LineStyle::solid,
            3.0,
            python_plotting::Color::blue,
            "Triangular (own x)"
        },
        {
            {0,1,2,3,4,5,6,7}, // This line's own x-axis values.
            {0,1,2,4,8,16,32,64},
            python_plotting::LineStyle::dashed,
            1.5,
            python_plotting::Color::red,
            "" // No label: left out of the legend.
        }
    };

    python_plotting::PythonPlotter plotter;
    if (verboseMode)
        plotter.setVerboseOutput(true);
    plotter.setLabels("MIATest Per-Line X Plot", "X-Values", "Y-Values");
    plotter.enableLegend(true);

    return plotter.plot(data);
}


/**
 * @brief Tests the plot() rejection paths for invalid input data.
 * @return SUCCESS when every invalid input is rejected, FAILURE otherwise.
 */
int testPythonPlotterValidation()
{
    std::vector<int> x = {0,1,2,3};
    bool allPassed = true;

    {
        // A line whose yValues do not match the shared x-axis size.
        python_plotting::LinesToPlot<int> badSize = {
            {
                {},
                {0,1,2},
                python_plotting::LineStyle::solid,
                1.0,
                python_plotting::Color::blue,
                "bad size"
            }
        };
        python_plotting::PythonPlotter plotter;
        bool result = plotter.plot(x, badSize);
        std::cout << "shared-axis size mismatch rejected: "
                  << (result ? "false" : "true") << std::endl;
        allPassed = allPassed && !result;
    }

    {
        // A line with empty xValues in the single-argument overload, which
        // requires every line to carry its own x-axis.
        python_plotting::LinesToPlot<int> noX = {
            {
                {},
                {0,1,2,3},
                python_plotting::LineStyle::solid,
                1.0,
                python_plotting::Color::blue,
                "no x"
            }
        };
        python_plotting::PythonPlotter plotter;
        bool result = plotter.plot(noX);
        std::cout << "empty xValues rejected in plot(data): "
                  << (result ? "false" : "true") << std::endl;
        allPassed = allPassed && !result;
    }

    {
        // Mismatched x/y sizes in the simple plot.
        python_plotting::PythonPlotter plotter;
        bool result = plotter.plot(std::vector<int>{0,1,2}, std::vector<int>{0,1});
        std::cout << "simple plot size mismatch rejected: "
                  << (result ? "false" : "true") << std::endl;
        allPassed = allPassed && !result;
    }

    std::cout << "Validation tests finished!" << std::endl;
    return allPassed ? constants::SUCCESS : constants::FAILURE;
}


int MIATest::run()
{
    //return testPythonModule();
    //return testPythonPlotter(getVerboseMode());
    //return testPythonPlotterMultiLine(getVerboseMode());
    //return testPythonPlotterPerLineX(getVerboseMode());
    return testPythonPlotterValidation();
}
