# Add a custom set of tests for the template application.
add_test( NAME TemplateApp_T
          COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/templateApp_T.sh
          $<TARGET_FILE:MIATemplate>
)

set_tests_properties(TemplateApp_T PROPERTIES
                     WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} 
)
