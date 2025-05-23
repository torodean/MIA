/**
 * File: MIADatabase.hpp
 * Author: Antonius Torode
 * Date: 12/23/2021
 * Description: This is a file for storing and dealing with credentials for a user.
 */

#include <string>
#include <utility>
#include <iostream>
#include <termios.h>
#include <unistd.h>

using std::cout;
using std::cin;

/**
 * This is a class for storing credentials for a user.
 */
class Credentials
{
public:
    /// Main default constructor for a Credentials object.
    Credentials() = default;
    ~Credentials()
    {
        clear();
    };

    Credentials(std::string user, std::string pw)
    {
        username = std::move(user);
        hash = hashPassword(std::move(pw));
    };

    /**
     * Set's the username of this object.
     * @param user std::string username to set.
     */
    void setUsername(std::string user)
    { username = std::move(user); }

    /**
     * Get's the username of this object.
     * @return user std::string username to return.
     */
    std::string getUsername()
    { return username; }

    /**
     * This will set the hash value from a possword.
     * @param pw std::string password to set the hash value from.
     */
    void setPasswordHash(std::string pw)
    { hash = hashPassword(std::move(pw)); }

    /**
     * This will set the possword.
     * @param pw std::string password to store.
     */
    void setPassword(std::string pw)
    { password = std::move(pw); }

    /**
     * This will return the possword.
     * @return std::string password value;
     */
    std::string getPassword()
    { return password; }

    /**
     * This will set the hash value to a hash that was already hashed from a password.
     * @param hashedPW
     */
    void setHash(std::string hashedPW)
    { hash = std::move(hashedPW); }

    /// Returns the hash of the password for this user's crednetials.
    std::string getHash()
    { return hash; }

    /**
     * The port value to set for this object.
     * @param p unsigned short& port value.
     */
    void setPort(unsigned short& p)
    { port = p; }

    /// Returns the port value.
    unsigned short getPort()
    { return port; }

    /**
     * Set's the hostname for this object.
     * @param host std::string hostname to set.
     */
    void setHostname(std::string& host)
    { hostname = host; }

    /// Returns the hostname for this credentials object.
    std::string getHostname()
    { return hostname; }

    /**
     * Clears the current user credential values.
     */
    void clear()
    {
        username.clear();
        hash.clear();
    };

    /**
     * Checks a password to see if it matches the user's hashed credentials.
     * @param in std::string of the password to check against.
     * @return true if hash matches, false otherwise.
     */
    bool checkPassword(std::string pw)
    { return hashPassword(std::move(pw)) == hash; }

    /**
     * Checks a hash to see if it matches the user's credentials.
     * @param hashedPW std::string of the hash to check against.
     * @return true if hash matches, false otherwise.
     */
    bool checkHash(const std::string& hashedPW)
    { return hashedPW == hash; }

    /**
     * In some cases, an actually password string is needed.
     * This method will query the user for their password to use. 
     * @param passwordUsage std::string describing what the password is needed for.
     * @return Returns the user password.
     */
    static std::string getPasswordFromUser(const std::string& passwordUsage = "login")
    {
        std::string password;
        cout << "Enter password for " << passwordUsage << ": ";
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
        // @TODO - make the password hidden in windows.
        /* Here's some test code from codedirect.com to test.
          HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
          DWORD mode = 0;
          GetConsoleMode(hStdin, &mode);
          SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));
         */
        getline(std::cin, password);
        // SetConsoleMode(hStdin, mode); //restore console to normal
#elif __linux__
        termios oldt{};
        tcgetattr(STDIN_FILENO, &oldt);
        termios newt = oldt;
        newt.c_lflag &= ~ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        getline(std::cin, password);
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif
        return password;
    }

private:
    /**
     * A method for turning a password into a hash string.
     * @param pw std::String password to hash.
     * @return returns the hash value.
     */
    std::string hashPassword(std::string pw)
    {
        //@TODO - write hashing algorithm.
        return pw;
    }

    unsigned short port{};      ///< Port number for connection using these credentials.
    std::string hostname;       ///< Hostname number for connection using these credentials.
    std::string username;       ///< The username of the credentials.
    std::string password;       ///< Used in some cases. For security, store hash instead.
    std::string hash;           ///< The hash associated with the password for the user.
};
