#!/usr/bin/env python3
"""
@file    PythonPlotter.py
@author  Antonius Torode
@date    09/09/2026

Python methods used for plotting.
"""

import matplotlib.pyplot as plt
import numpy as np

#########################
# CONFIGURATION VARIABLES
#########################
# Enables verbose output.
verboseMode = False

# Defines whether or not to show any plots after creating them.
showPlots = True
xAxisLabel = None
yAxisLabel = None
plotTitle = None

# Defines whether or not to draw a grid behind the plotted data.
showGrid = False

# The figure size as a (width, height) tuple in inches, or None for
# matplotlib's default size.
figureSize = None

# The data to plot when plotting multiple lines using the plotData method.
# Lines accumulate across plotData calls; every call plots every line added
# since the last clearData. This lets callers add lines over time and plot
# them together (or re-plot them), at the cost of clearing between plots
# when each plot should stand alone.
linesToPlot = []

# The x-axis values to plot the lines against. When unset, the lines are
# plotted against their index (matplotlib's default for single-array plots).
# This persists until replaced by setXAxis or cleared by clearData.
xAxis = None

#####################
# GETTERS AND SETTERS
#####################

def setVerbose(val):
    """
    Setter for the verboseMode variable.
    """
    global verboseMode
    verboseMode = val

    if verboseMode:
        print(f"Verbose mode enabled for PythonPlotter (python module).")

def setShowPlots(val):
    """
    Setter for the showPlots variable.
    """
    global showPlots
    showPlots = val

def setLabels(title, xLabel, yLabel):
    """
    Setter for the title and axis labels.
    """
    global plotTitle, xAxisLabel, yAxisLabel
    plotTitle = title
    xAxisLabel = xLabel
    yAxisLabel = yLabel

    if verboseMode:
        print(f"Plot labels set: title='{title}', x='{xLabel}', y='{yLabel}'")

def setXAxis(x):
    """
    Setter for the x-axis values used by the plotData method.
    """
    global xAxis
    xAxis = x

    if verboseMode:
        print(f"X-axis set with {len(x)} data points.")

def setShowGrid(val):
    """
    Setter for the showGrid variable.
    """
    global showGrid
    showGrid = val

def setFigureSize(width, height):
    """
    Setter for the figureSize variable, in inches.
    """
    global figureSize
    figureSize = (width, height)

    if verboseMode:
        print(f"Figure size set to {width} x {height} inches.")

#########################
# INTERNAL HELPER METHODS
#########################

def applyLabels():
    """
    Applies the configured plot title and axis labels.
    """
    if plotTitle is not None:
        plt.title(plotTitle)
    if xAxisLabel is not None:
        plt.xlabel(xAxisLabel)
    if yAxisLabel is not None:
        plt.ylabel(yAxisLabel)

def applyFigureSize():
    """
    Applies the configured figure size to the current figure.
    """
    if figureSize is not None:
        fig = plt.gcf()
        fig.set_size_inches(figureSize[0], figureSize[1])

def displayPlot():
    """
    Displays the current plot if plot display is enabled.
    """
    if showPlots:
        if verboseMode:
            print("Displaying plot.")

        plt.show()
        
##################
# PLOTTING METHODS
##################

def simplePlot(x, y):
    """
    Creates a simple plot of x values vs y values.
    """
    if verboseMode:
        print(f"Creating simple plot with {len(x)} data points.")

    xpoints = np.array(x)
    ypoints = np.array(y)

    # The figure size must be applied before the first plot call; applying it
    # afterwards has no effect on the already-created figure.
    applyFigureSize()
    plt.plot(xpoints, ypoints)
    plt.grid(showGrid)

    applyLabels()
    displayPlot()


def buildPlotData(values, lineStyle, color, lineWidth, xValues=None):
    """
    Adds a line to the accumulated plot data. The line is not drawn until
    plotData is called, and it remains in the plot data (and in every
    subsequent plot) until clearData removes it. When xValues is provided,
    the line is plotted against it instead of the shared x-axis.
    """
    linesToPlot.append({
        "values": values,
        "xValues": xValues,
        "lineStyle": lineStyle,
        "color": color,
        "lineWidth": lineWidth
    })

    if verboseMode:
        xInfo = f", {len(xValues)} x points" if xValues is not None else ""
        print(f"Added plot line: {len(values)} points{xInfo}, "
              f"style='{lineStyle}', color='{color}', "
              f"width={lineWidth}")


def plotData():
    """
    Plots all lines currently stored in linesToPlot. Lines are not consumed
    by plotting: they stay in linesToPlot and will appear in every subsequent
    plot until clearData removes them.
    """
    if verboseMode:
        print(f"Creating multi-line plot with {len(linesToPlot)} lines.")

    # The figure size must be applied before the first plot call; applying it
    # afterwards has no effect on the already-created figure.
    applyFigureSize()

    # Fall back to matplotlib's index default when no x-axis was provided.
    xAxisPoints = np.array(xAxis) if xAxis is not None else None

    for line in linesToPlot:
        # A line with its own xValues is plotted against those instead of the
        # shared x-axis.
        lineX = np.array(line["xValues"]) if line["xValues"] is not None else xAxisPoints
        values = np.array(line["values"])

        if lineX is None:
            plt.plot(values,
                     linestyle=line["lineStyle"],
                     color=line["color"],
                     linewidth=line["lineWidth"])
        else:
            plt.plot(lineX,
                     values,
                     linestyle=line["lineStyle"],
                     color=line["color"],
                     linewidth=line["lineWidth"])

    plt.grid(showGrid)
    applyLabels()
    displayPlot()


def clearData():
    """
    Clears the accumulated plot data: the stored lines and the shared x-axis.
    Call this between plots when each plot should stand alone. Labels are not
    cleared.
    """
    if verboseMode:
        print(f"Clearing {len(linesToPlot)} plot lines.")

    linesToPlot.clear()
    global xAxis
    xAxis = None
