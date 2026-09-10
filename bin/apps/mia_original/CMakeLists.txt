# Create the MIAOriginal executable.
set(MIAOriginal_SRC Commands.cpp MIAOriginal.cpp MIAOriginal_main.cpp )
set(MIAOriginal_INC Commands.hpp MIAOriginal.hpp )
add_executable(MIAOriginal ${MIAOriginal_SRC} ${MIAOriginal_INC} )
if (WIN32 OR CYGWIN)
	target_link_libraries(MIAOriginal PRIVATE Framework_CORE Types_UTIL Math_UTIL System_UTIL Files_UTIL Audio_UTIL Encryption_UTIL Windows_UTIL )
else()
	target_link_libraries(MIAOriginal PRIVATE Framework_CORE Types_UTIL Math_UTIL System_UTIL Files_UTIL Audio_UTIL Encryption_UTIL )
endif()

# Install the app.
install_app( MIAOriginal )
