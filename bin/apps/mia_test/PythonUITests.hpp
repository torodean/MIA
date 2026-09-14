/**
 * @file PythonUITests.hpp
 * @author Antonius Torode
 * @date 09/14/2026
 * @brief Declares the python UI test methods.
 */
#pragma once


/**
 * Tests basic python UI interactions using the in-file listener prototype.
 *
 * @return SUCCESS when the UI loop completes, FAILURE otherwise.
 */
int testPythonUI();

/**
 * Tests the python UI library's PythonUIListener. It behaves the same as
 * testPythonUI() except that the listener comes from the library and the
 * events are read through the EventStorage API.
 *
 * @return SUCCESS when the UI loop completes, FAILURE otherwise.
 */
int testPythonUILibrary();
