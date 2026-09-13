# PythonResult tests need no interpreter; PythonModule tests link Python and
# need PythonModuleTests.py next to the test binary.
add_executable(PythonResult_T PythonResult_T.cpp ../PythonResult.cpp ../PythonResult.hpp )
target_link_libraries(PythonResult_T PRIVATE Framework_CORE GTest::gtest_main)
target_include_directories(PythonResult_T PRIVATE ../)
add_test(NAME PythonResult_T COMMAND PythonResult_T )

add_executable(PythonModule_T PythonModule_T.cpp ../PythonModule.cpp ../PythonModule.hpp ../PythonResult.cpp ../PythonResult.hpp )
target_link_libraries(PythonModule_T PRIVATE Framework_CORE Python3::Python GTest::gtest_main)
target_include_directories(PythonModule_T PRIVATE ../)
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/PythonModuleTests.py
    ${CMAKE_CURRENT_BINARY_DIR}/PythonModuleTests.py
    COPYONLY
)
add_test(NAME PythonModule_T COMMAND PythonModule_T )
