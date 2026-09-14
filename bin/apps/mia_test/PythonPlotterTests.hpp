/**
 * @file PythonPlotterTests.hpp
 * @author Antonius Torode
 * @date 09/14/2026
 * @brief Declares the python plotting test methods.
 */
#pragma once


/**
 * Tests the PythonPlotter basic plotting method.
 *
 * @param verboseMode Enables verbose output in the plotter when true.
 * @return The result of the plot operation.
 */
int testPythonPlotter(bool verboseMode);

/**
 * Tests the PythonPlotter multi-line plotting method.
 *
 * @param verboseMode Enables verbose output in the plotter when true.
 * @return The result of the plot operation.
 */
int testPythonPlotterMultiLine(bool verboseMode);

/**
 * Tests the PythonPlotter per-line x-axis and the legend label rules.
 *
 * Each line carries its own xValues, so the shared x-axis is unused. One line
 * has an empty label to verify it is left out of the legend.
 *
 * @param verboseMode Enables verbose output in the plotter when true.
 * @return The result of the plot operation.
 */
int testPythonPlotterPerLineX(bool verboseMode);

/**
 * Tests the plot() rejection paths for invalid input data.
 *
 * @return SUCCESS when every invalid input is rejected, FAILURE otherwise.
 */
int testPythonPlotterValidation();
