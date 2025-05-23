#ifndef D3STRING_H
#define D3STRING_H
#include <string>

using namespace std;

class d3string
{
public:
    d3string();
    long sumOfDigits(string number);
    string zeroString(int length);
    int numOfDigits(string x);
    int spacesInAString(string text);
};

#endif // D3STRING_H
