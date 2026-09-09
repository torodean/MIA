#!/usr/bin/env python3
"""
@file    PythonModuleTests.py
@author  Antonius Torode
@date    09/08/2026

Python methods used by PythonModule_T.cpp to exercise the PythonModule wrapper.
Each method covers one argument signature or return type. This file is copied
next to the test binary so the wrapper can import it from the working directory.
"""


def add(a, b):
    """Returns the sum of two integers."""
    return a + b


def multiply(a, b):
    """Returns the product of two doubles."""
    return a * b


def greet(name):
    """Returns a greeting string for the given name."""
    return "Hello, " + name + "!"


def describe(name, age):
    """Returns a string mixing a string and an integer argument."""
    return name + " is " + str(age) + " years old."


def sumVals(values):
    """Returns a string containing the values and their sum."""
    return "Values list:" + str(values) + "; Sum:" + str(sum(values))

    
def doNothing():
    """Returns None to cover the void return path."""
    return None
