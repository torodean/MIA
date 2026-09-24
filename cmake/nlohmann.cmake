# Checking if nlohmann/json is available for the json requirements.
find_path(NLOHMANN_JSON_INCLUDE_DIR nlohmann/json.hpp)

if(NLOHMANN_JSON_INCLUDE_DIR)
    set(BUILD_NLOHMANN_JSON_FEATURES ON)
    message(STATUS "Found nlohmann/json. BUILDING nlohmann/json components!")
else()
    set(BUILD_NLOHMANN_JSON_FEATURES OFF)
    list(APPEND SKIPPED_FEATURES "nlohmann/json")
    message(STATUS "nlohmann/json not found. SKIPPING nlohmann/json components!")
endif()