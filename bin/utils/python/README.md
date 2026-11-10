# Utilities - Python

This folder holds the utilities used to write and run python code from within the MIA project.
It contains the c++ embedded python interpreter, for loading python from within code, and the shared python modules which python files throughout the project can import.

## Current Contents

- **PythonModule.hpp/.cpp**: The embedded interpreter used by c++ code. It starts the interpreter, puts the correct python directory on `sys.path`, imports the requested module, and provides the ability to call Python code from within c++ code.
- **PythonResult.hpp/.cpp**: The resulting data returned by python calls in `PythonModule`, holding the return value and the error state.
- **pythonTools.py**: A shared python tool module, holding general-purpose helpers for console output, labeling, and logging.

## Running Python From C++

A python file should sit beside the c++ file which uses it.
`PythonModule` calls `paths::getPythonDirToUse(__FILE__)` for the directory to import files from (the system install python directory, the release `resources/python` directory, or the calling c++ file's directory in the repo) and puts it in `sys.path` before importing.

## Importing The Shared Python Tools

Shared python modules such as `pythonTools.py` live in this folder and are imported by python files located throughout the repo.
Those python files run in three layouts, and the import must work in all of them:

1. From the repo while developing, where the files sit scattered across their repo directories.
2. From a system install, where all installed python files are copied into the install python directory (e.g., `/usr/local/mia/python`).
3. From a release/portable build, where they are copied into `<release>/resources/python`.

In the two installed layouts, every python file lands beside the shared tools in the same directory, so `import pythonTools` works on its own.
That directory is `sys.path[0]` for a standalone script.
Only the repo layout scatters the files, and that case is detected by the normal import failing to find the file.
Python files using the shared python utilities should begin with this import statement:

```python
try:
    import pythonTools
except ImportError:
    import os, sys
    d = os.path.dirname(os.path.abspath(__file__))
    while not os.path.isfile(os.path.join(d, "bin", "utils", "python", "pythonTools.py")) \
            and d != os.path.dirname(d):
        d = os.path.dirname(d)
    sys.path.insert(0, os.path.join(d, "bin", "utils", "python"))
    import pythonTools
```

The `try` branch succeeds instantly in the installed layouts, so the except only ever runs from files ran in the repo.

## Adding A New Shared Python Module

1. Add the file to this folder.
2. Register it with `install_python_files(...)` in this folder's `CMakeLists.txt` so it lands in the correct location for system installs and release builds. Until it is registered, the module resolves only when running from the repo.
3. Keep the file name unique across all installed python files. Matching names from different folders can overwrite each other.
