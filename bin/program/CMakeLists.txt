# Create the Program_LIB.
set(Program_SRC 
    Error.cpp 
    ErrorDescriptions.cpp )
set(Program_INC 
    Error.hpp 
    ErrorDescriptions.hpp 
    MIAException.hpp )
add_library(Program_LIB ${Program_SRC} ${Program_INC})
