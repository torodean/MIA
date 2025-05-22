
# Create the fishbot executable.
set(WoWFishbot_SRC WoWFishbot.cpp WoWFishbot_main.cpp )
set(WoWFishbot_INC WoWFishbot.hpp )
add_executable(WoWFishbot ${WoWFishbot_SRC} ${WoWFishbot_INC} )
target_link_libraries(WoWFishbot PRIVATE Core_LIB System_LIB)
