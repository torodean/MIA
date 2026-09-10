# Installs the specified target files into the system directory when performing 
# a system install, or into the release directory when building a release.
function(install_app)
    if(SYSTEM_INSTALL)    
        install(TARGETS ${ARGV} DESTINATION ${APP_INSTALL_LOCATION})
    elseif(RELEASE_BUILD)
        install(TARGETS ${ARGV} DESTINATION ${RELEASE_INSTALL_LOCATION})
    endif()
endfunction()
