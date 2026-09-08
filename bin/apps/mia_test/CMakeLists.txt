# Find Python development libraries.
find_package(Python3 REQUIRED COMPONENTS Development.Embed)

# Create the test executable.
set(test_SRC MIATest.cpp MIATest_main.cpp )
set(test_INC MIATest.hpp )

#  The target name "test" is reserved when CTest testing is enabled.
add_executable(miatest ${test_SRC} ${test_INC} )
target_link_libraries(miatest PRIVATE Framework_CORE Python3::Python )

# The python file must be next to the cmake with how it's currently imported.
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/MIATest.py
    ${CMAKE_CURRENT_BINARY_DIR}/MIATest.py
    COPYONLY
)
