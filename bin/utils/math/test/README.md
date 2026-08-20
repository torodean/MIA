# Utilities - Math - Tests

This folder holds the unit tests for the math utilities.

The `*_T.cpp` files are GoogleTest sources, one per utility module (`D3CMath_T.cpp`, `MathUtils_T.cpp`, `FinanceUtils_T.cpp`, etc).

They are not built as separate executables.
All of them compile into a single `Math_T` executable, declared in [CMakeLists.txt](CMakeLists.txt).
To add a test for a new module, add its `*_T.cpp` file to that `add_executable` line rather than creating a new target.
