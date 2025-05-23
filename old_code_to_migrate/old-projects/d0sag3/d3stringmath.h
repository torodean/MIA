#ifndef D3STRINGMATH_H
#define D3STRINGMATH_H
#include <string>
#include <vector>

using namespace std;

class d3stringmath
{
public:
    d3stringmath();
    long sumOfDigits(string number);
    vector<int> stringToVector(string a);
    string vectorToStringZeroTrim(vector<int> a);
    string resizeString(string a, string b);
    string addTwoStrings(string stringOne, string stringTwo);
    string multiplyTwoStrings(string stringOne, string stringTwo);
};

#endif // D3STRINGMATH_H
