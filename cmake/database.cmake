# Checking if SQL database libraries are available to build.
find_path(MYSQLCPPCONN_INCLUDE_DIR mysql_connection.h)
find_library(MYSQLCPPCONN_LIBRARY NAMES mysqlcppconn)

if (MYSQLCPPCONN_INCLUDE_DIR AND MYSQLCPPCONN_LIBRARY)
    set(BUILD_DATABASE_FEATURES ON)
    message(STATUS "Found MySQL Connector/C++. BUILDING database components!")
else()
    set(BUILD_DATABASE_FEATURES OFF)
    message(STATUS "MySQL Connector/C++ not found. SKIPPING database components!")
endif()
