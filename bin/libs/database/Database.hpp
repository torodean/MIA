/**
 * File: Database.hpp
 * Author: Antonius Torode
 * Date: 03/11/2021
 * Description: This is the file designed for connection and use with databases.
 *              The MIA database uses SQL.
 */
#pragma once

#include <vector>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include "mysql_connection.h"

#include "RuntimeContext.hpp"
#include "Credentials.hpp"
#include "DatabaseCredentials.hpp"


/**
 * This is the class designed for connection and use with databases.
 * The MIA database uses MYSQL.
 * Connector/C++ API Reference: https://dev.mysql.com/doc/dev/connector-cpp/8.0/
 */
class Database
{
public:

    /**
	 * This is the main default constructor for a Database object. If this constructor
	 * is used, setCredentials() must then be called before a connection to the database
	 * can be established.
	 */
    Database() = default;
	
	/**
	 * Constructs a Database object with the specified username, hostname, and port.
	 *
	 * @param username The username for database access.
	 * @param hostname The hostname of the database server.
	 * @param port The port number for the database connection.
	 */
	Database(const std::string& username, const std::string& hostname, unsigned short port)
		: credentials(username, hostname, port)
	{ }
	
	/**
	 * Constructs a Database object using a DatabaseCredentials object.
	 *
	 * @param credentials A DatabaseCredentials object containing connection details.
	 */
	Database(const DatabaseCredentials& credentials) : credentials(credentials)
	{ };
	
    /// The main destructor for a Database object.
    ~Database();
	
	/**
	 * Sets the credentials for this Database object.
	 *
	 * @param credentials A DatabaseCredentials object containing connection details.
	 */
	void setCredentials(const DatabaseCredentials& credentials)
	{ this->credentials = credentials; }

	/**
	 * Sets the credentials for this Database object using individual fields.
	 *
	 * @param username The username for database access.
	 * @param hostname The hostname of the database server.
	 * @param port The port number for the database connection.
	 */
	void setCredentials(const std::string& username, 
	                    const std::string& hostname, 
						unsigned short port)
	{ this->credentials = DatabaseCredentials(username, hostname, port); }

    /**
     * Connects to the database.
     * setCredentials() should be ran first to gather database settings from config.
     */
    int connect();

    /**
     * This will set which database to use.
     */
    void setDatabase(std::string& db)
	{ database = db; };

    void databaseInterface();

    /**
     * This will add a table to a database.
     * CREATE TABLE tableName (elements[0], elements[1], ...);
     * @param tableName The name of the table to use
     * @param elements a vector of elements that represent the various table values.
     * @example
     *      std::vector<std::string> elements =  {"id INT", "value VARCHAR(25)", "time TIME"};
     *      db2.addTable("timeTable", elements);
     */
    void addTable(const std::string& tableName, 
	              const std::vector<std::string>& elements);

    /**
     * This will add an element to a table.
     * INSERT INTO tableName (elements[0], elements[1], ...) VALUES (values[0], values[1], ...);
     * @param tableName The name of the table to use
     * @param elements a vector of elements that represent the various table values.
     * @param values a vector of values that matches the various elements.
     * @example
     *      std::vector<std::string> element =  {"id", "value", "time"};
     *      std::vector<std::string> value0 =  {"0", "'Value0'", "'11:12:13'"};
     *      db2.addElementToTable("timeTable", element, value0);

     */
    void addElementToTable(const std::string& tableName, 
	                       const std::vector<std::string>& elements, 
						   std::vector<std::string> values);

    /**
     * This will display the elements of the table and also return the displayed
     * results as a string.
     * SELECT * FROM tableName
     * @param tableName[const std::string&] - The name of the table to display.
     * @return [std::string] - The elements of the table. 
     */
    std::string viewTable(const std::string& tableName);

    /**
     * This will delete a table from the database.
     * DROP TABLE tableName;
     * @param tableName the table to delete
     */
    void deleteTable(const std::string& tableName);
	
	/**
	 * @brief Sets the given RuntimeContext into this instance's context pointer.
	 * 
	 * Allows this class to update its runtime variables by pointing to another
	 * RuntimeContext instance.
	 * 
	 * @param otherContext The RuntimeContext to point to.
	 */
	void setContext(const RuntimeContext& ctx) 
	{ context = &ctx; }

protected:
	
	/// The runtime context object to enable application output.
	const RuntimeContext* context{nullptr};
	
private:

    /**
     * This will print out the SQLException information for a thrown exception.
     * @param e the sql::SQLException& to print info about.
     */
    static void printExceptionInfo(sql::SQLException &e);

    DatabaseCredentials credentials;  ///< Credentials used to login to a database.
    std::string database;             ///< The database name to login to.
	
    /// MySQL values for the connection.
    sql::Driver *driver{};
    sql::Connection *con{};
    sql::Statement *stmt{};
    sql::ResultSet *res{};

};
