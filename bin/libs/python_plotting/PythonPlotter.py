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
# Defines whether or not to show any plots after creating them.
showPlots = True
xAxisLabel = None
yAxisLabel = None
plotTitle = None


#####################
# GETTERS AND SETTERS
#####################

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
    print("Setting labels")
    plotTitle = title
    xAxisLabel = xLabel
    yAxisLabel = yLabel


##################
# PLOTTING METHODS
##################

def simplePlot(x, y):
    """
    Creates a simple plot of x values vs y values.
    """
    xpoints = np.array(x)
    ypoints = np.array(y)
    
    plt.plot(xpoints, ypoints)
    
    print(f"plotTitle:{plotTitle}")
    
    if (plotTitle is not None):
        plt.title(plotTitle)
    if (xAxisLabel is not None):
        plt.xlabel(xAxisLabel)
    if (yAxisLabel is not None):
        plt.ylabel(yAxisLabel)
    
    if showPlots:
        plt.show()
