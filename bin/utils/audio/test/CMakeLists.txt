add_executable(Audio_T Playlist_T.cpp)
target_link_libraries(Audio_T PRIVATE Audio_UTIL GTest::gtest_main)
add_test(NAME Audio_T COMMAND Audio_T )
