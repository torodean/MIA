# Collect all piece source files.
file( GLOB PIECES_SRC CONFIGURE_DEPENDS
      "${CMAKE_CURRENT_SOURCE_DIR}/pieces/*.cpp" )

# Collect all piece header files.
file( GLOB PIECES_INC CONFIGURE_DEPENDS 
      "${CMAKE_CURRENT_SOURCE_DIR}/pieces/*.hpp" )

# Create the Check executable.
set( CHECK_SRC 
     Check.cpp 
     Check_main.cpp
     CheckConfig.cpp
     Piece.cpp
     PieceType.cpp
     MoveGeneration.cpp
     PlayerState.cpp
     SquareState.cpp
     BoardState.cpp
     EventHandler.cpp
     GameResult.cpp
     MoveState.cpp

     # Add the logic for the pieces.
     ${PIECES_SRC}
)
set( CHECK_INC 
     Check.hpp
     CheckConfig.hpp
     Piece.hpp
     PieceType.hpp
     MoveGeneration.hpp
     PlayerState.hpp
     SquareState.hpp
     BoardState.hpp
     EventHandler.hpp
     GameResult.hpp
     MoveState.hpp

     # Add the logic for the pieces.
     ${PIECES_INC}
)
add_executable( check ${CHECK_SRC} ${CHECK_INC} )
target_link_libraries( check
                   PRIVATE
                       Framework_CORE # For application framework.
                       Py_UI_LIB      # For UI interfacing.
                       Math_UTIL      # For coordinate handling.
                       Types_UTIL     # For string manipulation and parsing.
                       System_UTIL    # For timing.
                       Audio_UTIL     # For sound effects.
                     )

# Add this so that the files in the pieces folder don't need "../" for
# header includes (and visa versa).
target_include_directories(check PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/pieces
)

# Install the app.
install_app( check )

# Install the associated python files.
install_python_files( checkUI.py optionsUI.py )

# TODO - add tests.
#add_test_dir()
