#!/usr/bin/env python3

def main():
    print("Hello from Python!")


def add(a, b):
    """Returns the sum of two integers."""
    return a + b


def multiply(a, b):
    """Returns the product of two doubles."""
    return a * b


def greet(name):
    """Returns a greeting string for the given name."""
    return "Hello, " + name + "!"


def repeat(word, count):
    """Returns the word repeated count times, separated by spaces."""
    return " ".join([word] * count)


def describe(name, age):
    """Returns a string mixing a string and an integer argument."""
    return name + " is " + str(age) + " years old."


def printSum(a, b):
    """Prints the sum of two integers and returns nothing."""
    print("Sum: " + str(a + b))


if __name__ == "__main__":
    main()
