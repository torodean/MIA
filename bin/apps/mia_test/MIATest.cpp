/**
 * @file MIATest.cpp
 * @author Antonius Torode
 * @date 09/03/2026
 * @brief Implements any test features.
 */

#include <iostream>
<<<<<<< HEAD
#include <Python.h>
=======
>>>>>>> ef84e99 (Renaming folder case to lower)

// The associated header file.
#include "MIATest.hpp"


MIATest::MIATest()                      
{ };


void MIATest::initialize(int argc, char* argv[])
{
	// TODO
}

int MIATest::run()
{
	// Initialize the embedded Python interpreter.
    Py_Initialize();

    /*
	 * Add the current working directory to Python's module search path.
     * This allows Python to find MIATest.py when it is in the current directory.
	 * This is the path relative to where the executable runs.
	 */
    PyRun_SimpleString(
        "import sys\n"
        "sys.path.insert(0, '.')\n"
    );

    // Create a Python string containing the module name.
    PyObject* moduleName = PyUnicode_FromString("MIATest");

    // Import the MIATest Python module.
    PyObject* module = PyImport_Import(moduleName);
    Py_DECREF(moduleName);

    // Verify that the module was imported successfully.
    if (module == nullptr)
    {
        PyErr_Print();
        Py_Finalize();
        return 1;
    }

    // Retrieve the main() function from the Python module.
    PyObject* function = PyObject_GetAttrString(module, "main");

    // Verify that main() exists and is callable.
    if (function == nullptr || !PyCallable_Check(function))
    {
        PyErr_Print();
        Py_XDECREF(function);
        Py_DECREF(module);
        Py_Finalize();
        return 1;
    }

    // Call Python's main() function with no arguments.
    PyObject* result = PyObject_CallObject(function, nullptr);

    // Verify that the Python function executed successfully.
    if (result == nullptr)
    {
        PyErr_Print();
        Py_DECREF(function);
        Py_DECREF(module);
        Py_Finalize();
        return 1;
    }

    // Release the Python objects created above.
    Py_DECREF(result);
    Py_DECREF(function);
    Py_DECREF(module);

    // Shut down the embedded Python interpreter.
    Py_Finalize();

    std::cout << "Tests finished!" << std::endl;
    return constants::SUCCESS;
}
