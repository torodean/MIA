/**
 * @file PythonPlotter.hpp
 * @author Antonius Torode
 * @date 09/08/2026
 * @brief Declares a library for performing various plotting capabilities
 *        using python plotting calls.
 */
#pragma once

#include <vector>

// The interpreter for loading python modules and methods.
#include "PythonModule.hpp"


namespace python_plotting
{
    class PythonPlotter
    {
    public:
        PythonPlotter();
        ~PythonPlotter() = default;
        
        /**
         * @brief A simple method that just plots x vs y.
         * @note The vectors must be equal in size for this method to work.
         * @return true if the plotting was successful, false otherwise.
         */
        bool plot(std::vector<int> x, std::vector<int> y);
        
        void setLabels(const std::string& title,
                       const std::string& xLabel,
                       const std::string& yLabel);
        
    private:
        /// The name of the python module which contains the plotting methods.
        std::string moduleName{"PythonPlotter"};
        
        /**
         * The python module loader. This is constructed using the moduleName
         * set directly above and using the __FILE__ caller location since the
         * python modules are contained in the same code folder as this file.
         */
        PythonModule pythonModule;
    }; // class PythonPlotter
} // namespace python_plotting
