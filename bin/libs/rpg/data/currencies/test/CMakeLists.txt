add_executable(CurrencyRegistry_T CurrencyRegistry_T.cpp)
target_link_libraries(CurrencyRegistry_T PRIVATE RPG_Currencies_LIB GTest::gtest_main)
add_test(NAME CurrencyRegistry_T COMMAND CurrencyRegistry_T )

add_executable(CurrencyContainer_T CurrencyContainer_T.cpp)
target_link_libraries(CurrencyContainer_T PRIVATE RPG_Currencies_LIB GTest::gtest_main)
add_test(NAME CurrencyContainer_T COMMAND CurrencyContainer_T )

add_executable(Currency_T Currency_T.cpp)
target_link_libraries(Currency_T PRIVATE RPG_Currencies_LIB GTest::gtest_main)
add_test(NAME Currency_T COMMAND Currency_T )
