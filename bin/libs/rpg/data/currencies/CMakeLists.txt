
# Create the RPG_Currencies_LIB
set(RPG_Currencies_SRC 
    Currency.cpp
    Wallet.cpp )
set(RPG_Currencies_INC 
    Currency.hpp
    CurrencyType.hpp
    Wallet.hpp
    CurrencyRegistry.hpp )
add_library(RPG_Currencies_LIB ${RPG_Currencies_SRC} ${RPG_Currencies_INC})
target_link_libraries(RPG_Currencies_LIB PUBLIC System_UTIL RPG_Tools_LIB RPG_Data_LIB )

# Expose this library's source directory for #include access by dependent targets
target_include_directories(RPG_Currencies_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Include the test directory.
add_subdirectory( test )
