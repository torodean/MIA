# Adding the Database_LIB
set( Database_SRC Database.cpp )
set( Database_INC Database.hpp DatabaseCredentials.hpp )
add_library( Database_LIB ${Database_SRC} ${Database_INC} )
target_link_libraries( Database_LIB PUBLIC Framework_CORE Security_UTIL mysqlcppconn)

# Expose this library's source directory for #include access by dependent targets
target_include_directories( Database_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )

# Add the directory where tests are located.
add_subdirectory( test ) 
