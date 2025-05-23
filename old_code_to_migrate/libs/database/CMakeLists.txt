
set(Database_SRC MIADatabase.cpp)
set(Database_INC MIADatabase.hpp Credentials.hpp)

include_directories(../../program)
add_library(Database_LIB ${Database_SRC} ${Database_INC})