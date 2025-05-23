
set(Encryption_SRC 
    D3CEncrypt.cpp 
    D3CEncryptPW.cpp 
    MIAEncrypt.cpp )
set(Encryption_INC 
    D3CEncrypt.hpp 
    D3CEncryptPW.hpp 
    MIAEncrypt.hpp ) 
add_library(Encryption_LIB ${Encryption_SRC} ${Encryption_INC})
