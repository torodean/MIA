# Create the MIASequencer executable.
set(MIASequencer_SRC 
    MIASequencer.cpp
    MIASequencer_main.cpp
    Sequences.cpp )
set(MIASequencer_INC 
    MIASequencer.hpp
    Sequences.hpp )
add_executable(MIASequencer ${MIASequencer_SRC} ${MIASequencer_INC} )
target_link_libraries(MIASequencer PRIVATE Framework_CORE System_UTIL Types_UTIL Math_UTIL UI_UTIL )

# Install the app.
install_app( MIASequencer )
