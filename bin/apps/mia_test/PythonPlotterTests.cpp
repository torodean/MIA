/**
 * @file PythonPlotterTests.cpp
 * @author Antonius Torode
 * @date 09/14/2026
 * @brief Implements the python plotting test methods.
 */

#include <iostream>
#include <vector>

// The associated header file.
#include "PythonPlotterTests.hpp"
// Used for testing the PythonPlotter.
#include "PythonPlotter.hpp"
// Used for the return codes.
#include "Constants.hpp"


int testPythonPlotter(bool verboseMode)
{
    // Sequence to test plotting with.
    std::vector<int> x = {0,1,2,3,4,5,6,7};
    std::vector<int> y = {0,1,3,6,10,15,21,28};

    py_plotting::PythonPlotter plotter;
    if (verboseMode)
        plotter.setVerboseOutput(true);
    plotter.setLabels("MIATest Plot", "X-Values", "Y-Values");
    return plotter.plot(x,y);
}


int testPythonPlotterMultiLine(bool verboseMode)
{
    // Sequence to test plotting with.
    std::vector<int> x = {0,1,2,3,4,5,6,7};

    py_plotting::LinesToPlot<int> data = {
        {
            {}, // Empty xValues: use the shared x-axis.
            {0,1,3,6,10,15,21,28},
            py_plotting::LineStyle::solid,
            4.0,
            py_plotting::Color::blue,
            "Triangular numbers"
        },
        {
            {}, // Empty xValues: use the shared x-axis.
            {0,1,2,4,8,16,32,64},
            py_plotting::LineStyle::dashed,
            1.0,
            py_plotting::Color::red,
            "Powers of two"
        }
    };

    py_plotting::PythonPlotter plotter;
    if (verboseMode)
        plotter.setVerboseOutput(true);
    plotter.setLabels("MIATest Plot", "X-Values", "Y-Values");
    plotter.enableLegend(true);
    plotter.setShowGrid(true);
    plotter.setFigureSize(10.0, 6.0);

    return plotter.plot(x, data);
}


int testPythonPlotterPerLineX(bool verboseMode)
{
    py_plotting::LinesToPlot<int> data = {
        {
            {0,1,2,4,8,16,32,64}, // This line's own x-axis values.
            {0,1,3,6,10,15,21,28},
            py_plotting::LineStyle::solid,
            3.0,
            py_plotting::Color::blue,
            "Triangular (own x)"
        },
        {
            {0,1,2,3,4,5,6,7}, // This line's own x-axis values.
            {0,1,2,4,8,16,32,64},
            py_plotting::LineStyle::dashed,
            1.5,
            py_plotting::Color::red,
            "" // No label: left out of the legend.
        }
    };

    py_plotting::PythonPlotter plotter;
    if (verboseMode)
        plotter.setVerboseOutput(true);
    plotter.setLabels("MIATest Per-Line X Plot", "X-Values", "Y-Values");
    plotter.enableLegend(true);

    return plotter.plot(data);
}


int testPythonPlotterValidation()
{
    std::vector<int> x = {0,1,2,3};
    bool allPassed = true;

    {
        // A line whose yValues do not match the shared x-axis size.
        py_plotting::LinesToPlot<int> badSize = {
            {
                {},
                {0,1,2},
                py_plotting::LineStyle::solid,
                1.0,
                py_plotting::Color::blue,
                "bad size"
            }
        };
        py_plotting::PythonPlotter plotter;
        bool result = plotter.plot(x, badSize);
        std::cout << "shared-axis size mismatch rejected: "
                  << (result ? "false" : "true") << std::endl;
        allPassed = allPassed && !result;
    }

    {
        // A line with empty xValues in the single-argument overload, which
        // requires every line to carry its own x-axis.
        py_plotting::LinesToPlot<int> noX = {
            {
                {},
                {0,1,2,3},
                py_plotting::LineStyle::solid,
                1.0,
                py_plotting::Color::blue,
                "no x"
            }
        };
        py_plotting::PythonPlotter plotter;
        bool result = plotter.plot(noX);
        std::cout << "empty xValues rejected in plot(data): "
                  << (result ? "false" : "true") << std::endl;
        allPassed = allPassed && !result;
    }

    {
        // Mismatched x/y sizes in the simple plot.
        py_plotting::PythonPlotter plotter;
        bool result = plotter.plot(std::vector<int>{0,1,2}, std::vector<int>{0,1});
        std::cout << "simple plot size mismatch rejected: "
                  << (result ? "false" : "true") << std::endl;
        allPassed = allPassed && !result;
    }

    std::cout << "Validation tests finished!" << std::endl;
    return allPassed ? constants::SUCCESS : constants::FAILURE;
}
