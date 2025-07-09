add_executable(CurrencyRegistry_T CurrencyRegistry_T.cpp)
target_link_libraries(CurrencyRegistry_T PRIVATE RPG_Currencies_LIB GTest::gtest_main)
add_test(NAME CurrencyRegistry_T COMMAND CurrencyRegistry_T )

add_executable(Wallet_T Wallet_T.cpp)
target_link_libraries(Wallet_T PRIVATE RPG_Currencies_LIB GTest::gtest_main)
add_test(NAME Wallet_T COMMAND Wallet_T )

add_executable(Currency_T Currency_T.cpp)
target_link_libraries(Currency_T PRIVATE RPG_Currencies_LIB GTest::gtest_main)
add_test(NAME Currency_T COMMAND Currency_T )
