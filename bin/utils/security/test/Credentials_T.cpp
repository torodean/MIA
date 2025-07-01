#include <iostream>
#include "Credentials.hpp"

/**
 * This is a quick test to use for testing the password() feature of Credentials.hpp.
 * This has to be a standalone file which is not automated (not that it can't be) sopen
 * that the actual output '*' characters can be manually tested as they appear in terminal.
 */

int main() 
{
    std::string pw = credentials::password();
    std::cout << "You entered: [" << pw << "]" << std::endl;
    return 0;
}