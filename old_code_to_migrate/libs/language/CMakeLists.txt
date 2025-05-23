

set(Shengli_SRC Shengli.cpp)
set(Shengli_INC Shengli.hpp)

include_directories(../../program)
add_library(Shengli_LIB ${Shengli_SRC} ${Shengli_INC})
target_link_libraries(Shengli_LIB Program_LIB)

if (${USE_Database_LIB})
    message(STATUS "Linking language database.")
    target_link_libraries(Shengli_LIB Database_LIB)
else()
    message(STATUS "Skipping language database linking")
endif()