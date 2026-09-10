/**
 * @file PythonPlotter.hpp
 * @author Antonius Torode
 * @date 09/08/2026
 * @brief Declares a library for performing various plotting capabilities
 *        using python plotting calls.
 */
#pragma once

#include <vector>
#include <type_traits>
#include <variant>

// The interpreter for loading python modules and methods.
#include "PythonModule.hpp"


namespace python_plotting
{
    /**
     * @brief Defines the predefined colors available for plot lines.
     */
    enum Color
    {
        black,
        red,
        green,
        blue,
        yellow,
        orange,
        purple,
        white,
        gray
    };
    
    /**
     * @brief Converts a predefined Color to its string representation.
     * @note Returns "Unknown" if an unsupported color is entered.
     * @param color The predefined color to convert.
     * @return The color name as a string.
     */
    std::string ColorToString(const Color color);
    
    /**
     * The python plots can take a color by name or a hex-color code (string).
     * This variant will hold one or the other of those.
     */
    using PlotColor = std::variant<Color, std::string>;
    
    /**
     * @brief Validates a hexadecimal color string.
     * @note Valid colors must begin with '#' and contain 3, 4, 6, or 8
     *       (hexadecimal digits, corresponding to RGB, RGBA, RRGGBB, or RRGGBBAA).
     * @param color The color string to validate.
     * @return true if the string is a valid hexadecimal color, false otherwise.
     */
    bool validateColorString(const std::string& color);
    
    /**
     * @brief Converts a plot color to the format expected by the Python plotting module.
     * @param color The plot color, either a predefined Color or a custom hexadecimal
     *              color string beginning with '#'.
     * @return The color formatted as a string for use by the Python plotting module.
     */
    std::string getPythonFormattedColor(const PlotColor& color);
    
    /**
     * The valid line styles of the python plots.
     */
    enum LineStyle
    {
        solid,    ///< Corresponds to python's '-' line style. Default.
        dotted,   ///< Corresponds to python's ':' line style.
        dashed,   ///< Corresponds to python's '--' line style.
        dashdot   ///< Corresponds to python's '-.' line style.
    };
    
    /**
     * @brief Converts a LineStyle to the corresponding Python line style string.
     * @note Returns "Unknown" if an unsupported type is entered.
     * @param style The line style to convert.
     * @return The Python-formatted line style string.
     */
    std::string getPythonFormattedLineStyle(const LineStyle style);
    
    /**
     * Storage for metadata relevant for a line being plotted.
     * @tparam Type Numerical type of the data vectors.
     */
    template <typename Type>
    struct LineMetaData
    {
        std::vector<Type> xValues;         ///< The x-axis values for this data. When empty, the x-axis passed to plot() is used instead.
        std::vector<Type> yValues;         ///< The data to plot.
        LineStyle style{LineStyle::solid}; ///< The line style to use for this data.
        double lineWidth{1.5};             ///< The line width to use for this data.
        PlotColor color{Color::black};     ///< The color to use for this data.
        std::string label{};               ///< The label for this line in the plot legend.
    };

    /**
     * Defines a container which will hold all lines to be plotted.
     * @tparam Type Numerical type of the data vectors.
     */
    template <typename Type>
    using LinesToPlot = std::vector<LineMetaData<Type>>;

    /**
     * When plotting via the PythonPlotter module, each data vector must be the
     * same size as the x-axis vector it will be plotted against: the shared
     * x-axis for lines without their own xValues, or the line's xValues
     * otherwise. This method checks this.
     * @param x The shared x-axis data.
     * @param data The list of lines to plot.
     * @tparam Type Numerical type of the data vectors.
     */
    template <typename Type>
    bool validateDataSizesMatch(const std::vector<Type>& x,
                                const LinesToPlot<Type>& data)
    {
        for (const auto& dat : data)
        {
            const auto& xToUse = dat.xValues.empty() ? x : dat.xValues;
            if (dat.yValues.size() != xToUse.size())
                return false;
        }

        return true;
    }

    /**
     * When plotting via the PythonPlotter module, each data vector must be the
     * same size as its own x-axis vector. This overload checks this for plots
     * without a shared x-axis; every line must carry its own xValues, so an
     * empty xValues is also rejected.
     * @param data The list of lines to plot.
     * @tparam Type Numerical type of the data vectors.
     */
    template <typename Type>
    bool validateDataSizesMatch(const LinesToPlot<Type>& data)
    {
        for (const auto& dat : data)
        {
            if (dat.xValues.empty() || dat.yValues.size() != dat.xValues.size())
                return false;
        }

        return true;
    }

    /**
     * A plotter which creates plots through the Python plotting module.
     *
     * The module accumulates plotting state across calls: every multi-line
     * plot() call appends its lines to the module's stored data, and each
     * produced plot contains every line added since the last clear. Lines
     * accumulate until the module data is cleared, so a caller which wants
     * each plot drawn fresh must clear the data between plots.
     *
     * The module also holds the plot labels and x-axis values, which persist
     * across plots until replaced (labels) or cleared (x-axis).
     */
    class PythonPlotter
    {
    public:
        /**
         * @brief Constructs a PythonPlotter and loads the Python plotting module.
         *
         * Initializes the PythonModule using the configured module name and the
         * current source file path.
         */
        PythonPlotter();
        ~PythonPlotter() = default;
        
        /**
         * @brief A simple method that just plots x vs y.
         * @note The vectors must be equal in size for this method to work.
         * @note This will also use any labels set in the setLabels() method.
         * @param x The x-axis values to plot.
         * @param y The y-axis values to plot.
         * @return true if the plotting was successful, false otherwise.
         * @tparam Type Numerical type of the data vector.
         */
        template <typename Type>
        bool plot(const std::vector<Type>& x, const std::vector<Type>& y)
        {
            static_assert(std::is_arithmetic_v<Type>, 
                "Data vector must contain a numerical type.");
                
            if (x.size() != y.size())
                return false;

            pythonModule.call("simplePlot", x, y);
            return true;
        }
        
        /**
         * @brief Plots one or more lines.
         * @note Each line is plotted against its own xValues, or against the
         *       provided shared x-axis when its xValues is empty.
         * @note Each line may specify its own line style, line width, and color.
         * @note This will also use any labels set in the setLabels() method.
         * @note The lines are appended to the module's accumulated plot data;
         *       see the class comment for the state lifecycle.
         * @param x The shared x-axis values for lines without their own xValues.
         * @param data The lines and associated metadata to plot.
         * @return true if the plotting was successful, false if the data sizes do not match.
         * @tparam Type Numerical type of the x-axis and line data vectors.
         */
        template <typename Type>
        bool plot(const std::vector<Type>& x, const LinesToPlot<Type>& data)
        {
            if (!validateDataSizesMatch(x, data))
                return false;

            for (const auto& dat : data)
            {
                std::string color = getPythonFormattedColor(dat.color);
                std::string lineStyle = getPythonFormattedLineStyle(dat.style);

                if (dat.xValues.empty())
                {
                    pythonModule.call("buildPlotData", std::vector<Type>{}, dat.yValues, lineStyle, color, dat.lineWidth, dat.label);
                }
                else
                {
                    pythonModule.call("buildPlotData", dat.xValues, dat.yValues, lineStyle, color, dat.lineWidth, dat.label);
                }
            }

            pythonModule.call("setXAxis", x);
            pythonModule.call("plotData");

            return true;
        }

        /**
         * @brief Plots one or more lines, each against its own x-axis values.
         * @note Every line must have non-empty xValues; there is no shared
         *       x-axis in this overload.
         * @note Each line may specify its own line style, line width, and color.
         * @note This will also use any labels set in the setLabels() method.
         * @note The lines are appended to the module's accumulated plot data;
         *       see the class comment for the state lifecycle.
         * @param data The lines and associated metadata to plot.
         * @return true if the plotting was successful, false if any line has
         *         empty xValues or the data sizes do not match.
         * @tparam Type Numerical type of the line data vectors.
         */
        template <typename Type>
        bool plot(const LinesToPlot<Type>& data)
        {
            if (!validateDataSizesMatch(data))
                return false;

            for (const auto& dat : data)
            {
                std::string color = getPythonFormattedColor(dat.color);
                std::string lineStyle = getPythonFormattedLineStyle(dat.style);
                pythonModule.call("buildPlotData", dat.xValues, dat.yValues, lineStyle, color, dat.lineWidth, dat.label);
            }

            pythonModule.call("plotData");

            return true;
        }
        
        
        /**
         * @brief Sets the labels to be added to the produced plots.
         *
         * The labels persist across plots until replaced by a later call.
         *
         * @param title The plot title.
         * @param xLabel The label of the x-axis.
         * @param yLabel The label of the y-axis.
         */
        void setLabels(const std::string& title,
                       const std::string& xLabel,
                       const std::string& yLabel);

        /// Sets whether a grid is drawn behind the plotted data (off by default).
        void setShowGrid(bool val);

        /**
         * Sets whether the produced multi-line plots include a legend built
         * from the labeled lines (off by default).
         */
        void enableLegend(bool val);

        /**
         * Sets the figure size in inches. Only sizes set before the plot calls
         * take effect, since the Python module applies the size when creating
         * the figure.
         */
        void setFigureSize(double widthInches, double heightInches);


        /// Enables verbose output in the python modules.
        void setVerboseOutput(bool val);           
        
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
