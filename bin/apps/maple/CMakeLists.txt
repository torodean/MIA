# Create the Maple executable.
set(MAPLE_SRC 
    Maple.cpp 
    Maple_main.cpp
    TaxConstants.cpp 
    MoneyHandler.cpp
    FinancialCalculations.cpp 
    MapleMisc.cpp )
set(MAPLE_INC 
    Maple.hpp 
    MapleConstants.hpp
    TaxConstants.hpp
    MoneyHandler.hpp 
    FinancialCalculations.hpp 
    MapleMisc.hpp )
add_executable(maple ${MAPLE_SRC} ${MAPLE_INC} )
target_link_libraries(maple PRIVATE Framework_CORE System_UTIL Math_UTIL )

# Install the app.
install_app( maple )

add_subdirectory( test )
