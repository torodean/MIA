/**
 * File: DatabaseCredentials.hpp
 * Author: Antonius Torode
 * Date: 12/23/2021
 * Description: This is a file for storing and dealing with credentials for a user.
 */
#pragma once

#include <string>
#include <utility>
#include <iostream>
#include <termios.h>
#include <unistd.h>

/**
 * This is a class for storing credentials for a user.
 */
class Credentials
{
public:
    /// Main default constructor for a Credentials object.
    Credentials() = default;
	
	/// Main default constructor for a Credentials object.
    ~Credentials() = default;
	
	/**
     * Constructs a Credentials object with specified username, hostname, and port.
     *
     * @param user The username to set.
     * @param host The hostname to set.
     * @param port The port value to set.
     */
    Credentials(const std::string& user, 
	            const std::string& host, 
				const std::string& port) : 
		username(user), hostname(host), port(port)
    { };

    /**
     * Set's the username of this object.
     * @param user std::string username to set.
     */
    void setUsername(const std::string& user)
    { username = user; }

    /**
     * Get's the username of this object.
     * @return user std::string username to return.
     */
    std::string getUsername()
    { return username; }

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
		hostname.clear();
        username.clear();
        port = 0;
    };

private:
    unsigned short port{0};     ///< Port number for connection using these credentials.
    std::string hostname;       ///< Hostname number for connection using these credentials.
    std::string username;       ///< The username of the credentials.
};
