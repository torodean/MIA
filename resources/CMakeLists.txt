set(MIA_Config_Files
    MIAConfig.MIA
    excuses.MIA
    MIAWorkout.MIA
    WoWConfig.MIA
)

if(SYSTEM_INSTALL)
    install(FILES ${MIA_Config_Files} DESTINATION ${DEFAULT_SYSTEM_CONFIG_FILE_DIR})
endif()

if(RELEASE_BUILD)
    install(FILES ${MIA_Config_Files} DESTINATION ${RELEASE_CONFIG_INSTALL_LOCATION})
endif()
