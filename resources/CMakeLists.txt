set(MIA_Config_Files
    MIAConfig.MIA
    excuses.MIA
    MIAWorkout.MIA
    WoWConfig.MIA
)

install(FILES ${MIA_Config_Files} DESTINATION ${DEFAULT_SYSTEM_CONFIG_FILE_DIR})
