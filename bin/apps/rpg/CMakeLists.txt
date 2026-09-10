# Create the RPGFiddler executable.
set(RPGFiddler_SRC RPGFiddler.cpp RPGFiddler_main.cpp RPGSimulator.cpp )
set(RPGFiddler_INC RPGFiddler.hpp RPGSimulator.hpp )
add_executable(RPGFiddler ${RPGFiddler_SRC} ${RPGFiddler_INC} )
target_link_libraries( RPGFiddler PRIVATE Framework_CORE Files_UTIL RPG_DataLoader_LIB RPG_Player_LIB )

# Install the app.
install_app( RPGFiddler )
