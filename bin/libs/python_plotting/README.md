# Python Plotting Library

This folder contains a library for creating plots from c++ code. Plotting is performed by calling python plotting methods through the embedded python interpreter, with the python side contained in this folder.

## Current Contents

- **PythonPlotter.hpp/.cpp**: The plotting wrapper exposed to c++ code. It defines the `py_plotting` namespace, the `Color`/`LineStyle`/`PlotColor`/`LineMetaData` plotting types, and the `PythonPlotter` class. Plotting is delegated to the matching methods in `PythonPlotter.py`.
- **PythonPlotter.py**: The python module which performs the actual plotting with `matplotlib` (and `numpy`). Holds the plotting state (accumulated lines, labels, figure size) between calls and applies it when a plot is drawn.

The library is built as the `Py_Plotting_LIB` CMake target (only when the python features are available) and links to `Python_UTIL` for the embedded interpreter. See the MIA manual's Python Plotting Library section for usage examples.

## Structure Notes

The c++ wrapper and the python module are pairs: each wrapper method calls one python method which does the plotting work. This folder is expected to grow additional python plotting files, with new wrappers around them as needed, rather than funneling everything through the single `PythonPlotter`.
