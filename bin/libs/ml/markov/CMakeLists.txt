
# Create the Markov_LIB
set(Markov_SRC 
    AbstractMarkovModel.cpp 
    CharacterMarkovModel.cpp )
set(Markov_INC 
    AbstractMarkovModel.hpp 
    CharacterMarkovModel.hpp )
add_library(Markov_LIB ${Markov_SRC} ${Markov_INC})
target_link_libraries( Markov_LIB PUBLIC ML_UTIL Types_UTIL )

# Expose this library's source directory for #include access by dependent targets
target_include_directories(Markov_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Include the test directory.
add_subdirectory( test )
