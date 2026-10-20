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
     GameResult.cpp

     # Add the logic for the pieces.
     pieces/Pawn.cpp
     pieces/Rook.cpp
     pieces/Bishop.cpp
     pieces/Knight.cpp
     pieces/Queen.cpp
     pieces/King.cpp
)
set( CHECK_INC 
     Check.hpp
     CheckConfig.hpp
     Piece.hpp
     PlayerState.hpp
     SquareState.hpp
     BoardState.hpp
     EventHandler.hpp
     GameResult.hpp
     MoveState.hpp
     
     # Add the logic for the pieces.
     pieces/Pawn.hpp
     pieces/Rook.hpp
     pieces/Bishop.hpp
     pieces/Knight.hpp
     pieces/Queen.hpp
     pieces/King.hpp
)
