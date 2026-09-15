# cmake

This folder contains CMake modules which are included by the top-level `CMakeLists.txt` via `include( cmake/<module>.cmake )`. Breaking this logic out keeps the top-level file focused on project definition, paths, and subdirectories.

## Module Descriptions

### `apps.cmake`
Defines the `install_app` function, which installs app targets into the system or release install location depending on the build options.

### `database.cmake`
Finds MySQL Connector/C++ and sets `BUILD_DATABASE_FEATURES`.

### `python.cmake`
Finds the Python development libraries, sets `BUILD_PYTHON_FEATURES`, and defines the `install_python_files` function, which installs the given python files into the system python directory during a system install, or into the release python resources directory during a release build.

### `xdo.cmake`
Finds libxdo and X11 and sets `BUILD_XDO_FEATURES`.

## Adding a new optional dependency

The feature-detection modules (`database.cmake`, `python.cmake`, `xdo.cmake`, etc) share a common shape: each soft-finds its libraries (no `REQUIRED`), sets its `BUILD_*_FEATURES` flag accordingly, appends its feature name to the `SKIPPED_FEATURES` list when the libraries are not found, and prints a configure-time status message. To add a new optional dependency, create a module following this pattern, include it from the top-level `CMakeLists.txt`, and guard the targets which need it with the flag.
