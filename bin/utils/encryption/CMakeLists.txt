
set(Encryption_SRC 
    D3CEncrypt.cpp 
    D3CEncryptPW.cpp 
    MIAEncrypt.cpp )
set(Encryption_INC 
    D3CEncrypt.hpp 
    D3CEncryptPW.hpp 
    MIAEncrypt.hpp ) 
add_library(Encryption_UTIL ${Encryption_SRC} ${Encryption_INC})

# Expose this library's source directory for #include access by dependent targets
target_include_directories(Encryption_UTIL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
