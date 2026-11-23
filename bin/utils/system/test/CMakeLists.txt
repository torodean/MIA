add_executable(Timing_T Timing_T.cpp)
target_link_libraries(Timing_T PRIVATE System_UTIL Testing_CORE)
add_test(NAME Timing_T COMMAND Timing_T )

add_executable(CommandLine_T CommandLine_T.cpp)
target_link_libraries(CommandLine_T PRIVATE System_UTIL Testing_CORE Constants_CORE)
add_test(NAME CommandLine_T COMMAND CommandLine_T )

# The pipe reader tests use POSIX file descriptors, so they are only built on Linux.
if(NOT WIN32 AND NOT CYGWIN)
    add_executable(PipeReader_T PipeReader_T.cpp)
    target_link_libraries(PipeReader_T PRIVATE System_UTIL Testing_CORE)
    add_test(NAME PipeReader_T COMMAND PipeReader_T )
endif()
