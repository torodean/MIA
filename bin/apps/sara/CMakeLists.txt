# Create the sara executable.
set( SARA_SRC 
     Sara.cpp
     Sara_main.cpp
     Sequences.cpp )
set( SARA_INC 
     Sara.hpp
     Sequences.hpp )
add_executable( sara ${SARA_SRC} ${SARA_INC} )
target_link_libraries( sara
                   PRIVATE
                       Framework_CORE
                       Error_CORE
                       System_UTIL
                       Types_UTIL
                       Math_UTIL
                       UI_UTIL )

# Install the app.
install_app( sara )
