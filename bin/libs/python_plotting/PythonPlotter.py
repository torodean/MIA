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

# The data to plot when plotting multiple lines using the plotData method.
linesToPlot = []

# The x-axis values to plot the lines against. When unset, the lines are
# plotted against their index (matplotlib's default for single-array plots).
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
    
    plt.plot(xpoints, ypoints)
    
    applyLabels()    
    displayPlot()


def buildPlotData(values, lineStyle, color, lineWidth):
    """
    Adds a line and its metadata to the current plot.
    """
    linesToPlot.append({
        "values": values,
        "lineStyle": lineStyle,
        "color": color,
        "lineWidth": lineWidth
    })

    if verboseMode:
        print(f"Added plot line: {len(values)} points, "
              f"style='{lineStyle}', color='{color}', "
              f"width={lineWidth}")


def plotData():
    """
    Plots all lines currently stored in linesToPlot.
    """
    if verboseMode:
        print(f"Creating multi-line plot with {len(linesToPlot)} lines.")

    # Fall back to matplotlib's index default when no x-axis was provided.
    xAxisPoints = np.array(xAxis) if xAxis is not None else None

    for line in linesToPlot:
        values = np.array(line["values"])
        if xAxisPoints is None:
            plt.plot(values,
                     linestyle=line["lineStyle"],
                     color=line["color"],
                     linewidth=line["lineWidth"])
        else:
            plt.plot(xAxisPoints,
                     values,
                     linestyle=line["lineStyle"],
                     color=line["color"],
                     linewidth=line["lineWidth"])

    applyLabels()
    displayPlot()


def clearData():
    """
    Clears the current plotting data. This will only clear the data
    (x axis and y axis values) and not labels.
    """
    if verboseMode:
        print(f"Clearing {len(linesToPlot)} plot lines.")

    linesToPlot.clear()
    global xAxis
    xAxis = None
