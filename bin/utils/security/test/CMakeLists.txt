add_executable( Credentials_T Credentials_T.cpp ../Credentials.hpp ../Credentials.cpp )
target_link_libraries( Credentials_T PRIVATE Security_UTIL )
target_include_directories( Credentials_T PRIVATE ../ )
