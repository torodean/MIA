/**
 * @file PythonPlotter.cpp
 * @author Antonius Torode
 * @date 09/09/2026
 * @brief Implements the PythonPlotter methods.
 */

// Includes the associated header file.
#include "PythonPlotter.hpp"
 
#include <iostream>

namespace python_plotting
{
    PythonPlotter::PythonPlotter() :
        pythonModule(moduleName, __FILE__)
    {}
    
    
    void PythonPlotter::setVerboseOutput(bool val)
    {
        pythonModule.call("setVerbose", val);
    }
    
    
    void PythonPlotter::setLabels(const std::string& title,
                                  const std::string& xLabel,
                                  const std::string& yLabel)
    {
        pythonModule.call("setLabels", title, xLabel, yLabel);
    }
    
    
    std::string ColorToString(const Color color)
    {
        switch(color)
        {
            case Color::black:  return "black";
            case Color::red:    return "red";
            case Color::green:  return "green";
            case Color::blue:   return "blue";
            case Color::yellow: return "yellow";
            case Color::orange: return "orange";
            case Color::purple: return "purple";
            case Color::white:  return "white";
            case Color::gray:   return "gray";
            default:            return "Unknown";
        }
    }
    
    
    std::string getPythonFormattedLineStyle(const LineStyle style)
    {
        switch (style)
        {
            case LineStyle::solid:   return "-";
            case LineStyle::dotted:  return ":";
            case LineStyle::dashed:  return "--";
            case LineStyle::dashdot: return "-.";
            default:                 return "Unknown";
        }
    }
    
    
    bool validateColorString(const std::string& color)
    {
        if (color.empty() || color[0] != '#')
            return false;

        const std::size_t length = color.size();

        if (length != 4 && length != 5 && length != 7 && length != 9)
            return false;

        for (std::size_t i = 1; i < length; ++i)
        {
            if (!std::isxdigit(static_cast<unsigned char>(color[i])))
                return false;
        }

        return true;
    }

    
    
    std::string getPythonFormattedColor(const PlotColor& color)
    {
        if (std::holds_alternative<Color>(color))
        { // color is stored as a Color enum.
            return ColorToString(std::get<Color>(color));
        }
        else
        { // color is stored as a string.
            std::string colorString = std::get<std::string>(color);
            if (validateColorString(colorString))
            { // color is valid.
                return colorString;
            }
            else
            { // color is not valid. Fallback to black as default.
                // TODO - maybe handle this error case differently.
                std::cerr << "WARNING: Invalid python color entered, defaulting to black." 
                          << std::endl;
                return ColorToString(Color::black);
            }
        }
    }
} // namespace python_plotting
