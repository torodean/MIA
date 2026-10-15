# Create the Check executable.
set( CHECK_SRC Check.cpp Check_main.cpp )
set( CHECK_INC Check.hpp )
add_executable( Check ${CHECK_SRC} ${CHECK_INC} )
target_link_libraries( Check
                   PRIVATE
                       Framework_CORE # For application framework.
                       Py_UI_LIB      # For UI interfacing.
                     )

# Install the app.
install_app( Check )

# TODO - add tests.
#add_test_dir()
