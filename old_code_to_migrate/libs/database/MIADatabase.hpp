/**
 * File: MIADatabase.hpp
 * Author: Antonius Torode
 * Date: 03/11/2021
 * Description: This is the file designed for connection and use with databases.
 *              The MIA database uses SQL.
 */

#include "Credentials.hpp"

#include <vector>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using std::string;
using std::vector;

/**
 * This is the class designed for connection and use with databases.
 * The MIA database uses MYSQL.
 * Connector/C++ API Reference: https://dev.mysql.com/doc/dev/connector-cpp/8.0/
 */
class MIADatabase
{
public:

    /// This is the main default constructor for a MIADatabase object.
    MIADatabase() = default;
    /// The main destructor for a MIADatabase object.
    ~MIADatabase();

    /**
     * Initializes the database variables from the MIAConfig.MIA file.
     * This will setup the credentials and database to use.
     */
    void initialize();

    /**
     * Connects to the database.
     * Initialize should be ran first to gather database settings from config.
     */
     int connect();

    /**
     * Creates a test connection to a database. Used for testing.
     * The following setup needs used within mysql for this test.
     * mysql> CREATE DATABASE testDB;
     * mysql> CREATE USER testUser@localhost IDENTIFIED BY 'password'
     * mysql> GRANT ALL ON *.* TO 'testUser'@'localhost';
     * mysql> USE testDB;
     * mysql> CREATE TABLE testTable (id INT, value VARCHAR(150));
     * mysql> INSERT INTO testTable (id, value) VALUES (1, 'val01'), (2, 'val02'), (3, 'val03');
     * mysql> SELECT * FROM testTable # Use this to show the table values.
     * @return returns EXIT_SUCCESS if no failures occur.
     */
    int testConnect();

    /**
     * Runs some test code against the test database.
     * @return returns EXIT_SUCCESS if no failures occur.
     */
    int testDatabase();

    /**
     * This will set which database to use.
     */
    void setDatabase(string& db);

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
    void addTable(const string& tableName, const vector<string>& elements);

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
    void addElementToTable(const string& tableName, const vector<string>& elements, vector<string> values);

    /**
     * This will display the elements of the table.
     * SELECT * FROM tableName
     * @param tableName
     * @example
     *      db2.viewTable("timeTable");
     */
    void viewTable(const string& tableName);

    /**
     * This will delete a table from the database.
     * DROP TABLE tableName;
     * @param tableName the table to delete
     */
    void deleteTable(const string& tableName);

protected:

private:

    /**
     * This will print out the SQLException information for a thrown exception.
     * @param e the sql::SQLException& to print info about.
     */
    static void printExceptionInfo(sql::SQLException &e);

    Credentials credentials;    ///< Credentials used to login to a database.
    string database;            ///< The database name to login to.
    /// MySQL values for the connection.
    sql::Driver *driver{};
    sql::Connection *con{};
    sql::Statement *stmt{};
    sql::ResultSet *res{};

};
