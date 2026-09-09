/**
 * @file PythonPlotter.cpp
 * @author Antonius Torode
 * @date 09/09/2026
 * @brief Implements the PythonPlotter methods.
 */

// Includes the associated header file.
#include "PythonPlotter.hpp"
 
namespace python_plotting
{
    PythonPlotter::PythonPlotter() :
        pythonModule(moduleName, __FILE__)
    {}
    
    
    void PythonPlotter::setLabels(const std::string& title,
                                  const std::string& xLabel,
                                  const std::string& yLabel)
    {
        pythonModule.call("setLabels", title, xLabel, yLabel);
    }  
    
    
    bool PythonPlotter::plot(std::vector<int> x, std::vector<int> y)
    {
        if (x.size() != y.size())
            return false;
            
        pythonModule.call("simplePlot", x, y);
        return true;
    }
} // namespace python_plotting
