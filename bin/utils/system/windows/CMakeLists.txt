


if(WIN32 OR CYGWIN)
	message(STATUS "Compiling Windows only libraries...")
    set(Windows_SRC LOLANetUse.cpp)
    set(Windows_INC LOLANetUse.hpp)

    add_library(Windows_LIB ${Windows_SRC} ${Windows_INC})
	target_link_libraries(Windows_LIB netapi32)
endif()
