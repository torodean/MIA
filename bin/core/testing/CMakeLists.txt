# Create Testing_CORE.
set( Testing_SRC
     TestingUtils.cpp )
set( Testing_SRC
     TestingUtils.hpp )
add_library( Testing_CORE ${Testing_SRC} ${Testing_SRC} )
target_link_libraries( Testing_CORE
                   PUBLIC                # Allows these to propogate upstream to tests.
                       Error_CORE        # For Error and Exception handling.
                       GTest::gtest_main # Google testing framework.
                     )

# Expose this library's source directory for #include access by dependent targets.
target_include_directories( Testing_CORE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )

# Add unit tests.
add_test_dir()
