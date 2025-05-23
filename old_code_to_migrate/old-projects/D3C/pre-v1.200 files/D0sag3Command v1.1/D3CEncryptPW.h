#ifndef D3CENCRYPTPW_H
#define D3CENCRYPTPW_H

#include <string>
#include <vector>

class D3CEncryptPW
{
public:
    D3CEncryptPW();
    std::string EncryptPW(std::string message, std::string password);
    std::string DecryptPW(std::string message, std::string password);
    std::vector<int> stringToVector(std::string a);
    std::string PWRepeat(std::string PW, int PWR, int PWM);
    std::string PWmessageCombine(std::string message, std::string PW);
    std::string PWmessageUnCombine(std::string message, std::string PW);
    std::string invertString(std::string str);
};

#endif // D3CENCRYPTPW_H
