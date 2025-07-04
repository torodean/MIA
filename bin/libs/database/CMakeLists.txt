# Adding the Database_LIB
set( Database_SRC Database.cpp )
set( Database_INC Database.hpp DatabaseCredentials.hpp )
add_library( Database_LIB ${Database_SRC} ${Database_INC} )

# Add the directory where tests are located.
add_subdirectory( test ) 