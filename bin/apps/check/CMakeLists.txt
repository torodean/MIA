# Create the Check executable.
set( CHECK_SRC 
     Check.cpp 
     Check_main.cpp
     CheckConfig.hpp )
set( CHECK_INC 
     Check.hpp
     CheckConfig.hpp
     Piece.hpp
     PlayerState.hpp
     SquareState.hpp
     BoardState.hpp )
add_executable( Check ${CHECK_SRC} ${CHECK_INC} )
target_link_libraries( Check
                   PRIVATE
                       Framework_CORE # For application framework.
                       Py_UI_LIB      # For UI interfacing.
                       Math_UTIL      # For coordinate handling.
                       Types_UTIL     # For string manipulation and parsing.
                     )

# Install the app.
install_app( Check )

# TODO - add tests.
#add_test_dir()
