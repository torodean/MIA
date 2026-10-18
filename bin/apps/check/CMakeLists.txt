# Create the Check executable.
set( CHECK_SRC 
     Check.cpp 
     Check_main.cpp
     CheckConfig.cpp
     Piece.cpp
     PlayerState.cpp
     SquareState.cpp
     BoardState.cpp
     EventHandler.cpp
     Pawn.cpp )
set( CHECK_INC 
     Check.hpp
     CheckConfig.hpp
     Piece.hpp
     PlayerState.hpp
     SquareState.hpp
     BoardState.hpp
     EventHandler.hpp
     MoveState.hpp
     Pawn.hpp )
add_executable( check ${CHECK_SRC} ${CHECK_INC} )
target_link_libraries( check
                   PRIVATE
                       Framework_CORE # For application framework.
                       Py_UI_LIB      # For UI interfacing.
                       Math_UTIL      # For coordinate handling.
                       Types_UTIL     # For string manipulation and parsing.
                       System_UTIL    # For timing.
                     )

# Install the app.
install_app( check )

# TODO - add tests.
#add_test_dir()
