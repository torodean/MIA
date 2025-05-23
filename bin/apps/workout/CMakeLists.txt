# Create the MIAWorkout executable.
set(MIAWorkout_SRC MIAWorkout.cpp MIAWorkout_main.cpp )
set(MIAWorkout_INC MIAWorkout.hpp )
add_executable(MIAWorkout ${MIAWorkout_SRC} ${MIAWorkout_INC} )
target_link_libraries(MIAWorkout PRIVATE Core_LIB Types_LIB Math_LIB)
