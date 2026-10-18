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

     # Add the logic for the pieces.
     pieces/Pawn.cpp
     pieces/Rook.cpp
     pieces/Bishop.cpp
     pieces/Knight.cpp
     pieces/Queen.cpp
)
set( CHECK_INC 
     Check.hpp
     CheckConfig.hpp
     Piece.hpp
     PlayerState.hpp
     SquareState.hpp
     BoardState.hpp
     EventHandler.hpp
     MoveState.hpp
     
     # Add the logic for the pieces.
     pieces/Pawn.hpp
     pieces/Rook.hpp
     pieces/Bishop.hpp
     pieces/Knight.hpp
     pieces/Queen.hpp
     pieces/King.hpp
)
add_executable( check ${CHECK_SRC} ${CHECK_INC} )
target_link_libraries( check
                   PRIVATE
                       Framework_CORE # For application framework.
                       Py_UI_LIB      # For UI interfacing.
                       Math_UTIL      # For coordinate handling.
                       Types_UTIL     # For string manipulation and parsing.
                       System_UTIL    # For timing.
                     )

# Add this so that the files in the pieces folder don't need "../" for
# header includes (and visa versa).
target_include_directories(check PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/pieces
)

# Install the app.
install_app( check )

# TODO - add tests.
#add_test_dir()
