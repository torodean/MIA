#!/bin/python3

import re
import os
import sys


def increment_version(version_str):
    """
    Increment the minor part of a version string like 'Version 1.005' or '2.005'.

    Args:
        version_str (str): The current version string.

    Returns:
        str: The incremented version string (e.g., 'Version 1.006' or '2.006').
    """
    prefix, number = re.match(r'(.*?)(\d+\.\d+)', version_str).groups()
    major, minor = number.split('.')
    new_minor = f"{int(minor) + 1:03d}"
    return f"{prefix}{major}.{new_minor}"


def update_latex_version(filename):
    """
    Update the version number in a LaTeX file by incrementing it.

    Args:
        filename (str): Path to the LaTeX file.

    Returns:
        str: The new version string (e.g., '1.008').
    """
    with open(filename, 'r') as file:
        content = file.read()

    pattern = r'(\\newcommand{\\version}{)(Version\s+\d+\.\d+)(})'
    match = re.search(pattern, content)

    if match:
        old_version = match.group(2)
        new_version = increment_version(old_version)
        new_content = re.sub(pattern, rf'\1{new_version}\3', content)

        with open(filename, 'w') as file:
            file.write(new_content)
        print(f"LaTeX version updated: {old_version} → {new_version}")
        return new_version.split()[1]  # return just the number part
    else:
        print("Version line not found in LaTeX file.")
        sys.exit(1)


def update_cmake_version(filename):
    """
    Update the version number in a CMake file by incrementing it.

    Args:
        filename (str): Path to the CMake file.

    Returns:
        str: The new version string (e.g., '2.003').
    """
    with open(filename, 'r') as file:
        content = file.read()

    pattern = r'(set\s*\(\s*MIA_VERSION_VAL\s*")(\d+\.\d+)(")'
    match = re.search(pattern, content)

    if match:
        old_version = match.group(2)
        new_version = increment_version(old_version)
        new_content = re.sub(pattern, lambda m: f'{m.group(1)}{new_version}{m.group(3)}', content)

        with open(filename, 'w') as file:
            file.write(new_content)
        print(f"CMake version updated: {old_version} → {new_version}")
        return new_version
    else:
        print("Version line not found in CMake file.")
        sys.exit(1)


def verify_versions_match(version1, version2):
    """
    Ensure two version strings match. Exit with error if they do not.

    Args:
        version1 (str): First version string.
        version2 (str): Second version string.
    """
    if version1 != version2:
        print(f"Version mismatch: LaTeX = {version1}, CMake = {version2}")
        sys.exit(1)
        

# --- Execution ---

script_dir = os.path.dirname(os.path.abspath(__file__))
latex_path = os.path.abspath(os.path.join(script_dir, '..', 'docs/MIAManual.tex'))
cmake_path = os.path.abspath(os.path.join(script_dir, '..', 'CMakeLists.txt'))

latex_version = update_latex_version(latex_path)
cmake_version = update_cmake_version(cmake_path)
verify_versions_match(latex_version, cmake_version)

