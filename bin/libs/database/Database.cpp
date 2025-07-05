/**
 * File: Database.cpp
 * Author: Antonius Torode
 * Date: 03/11/2021
 * Description: This file contains the database connections source code.
 */

#include <iostream>
#include <sstream>

#include "Database.hpp"

Database::~Database()
{
    delete res;
    delete stmt;
    delete con;
}


int Database::connect()
{
    try
    {
        /* Create a connection */
        driver = get_driver_instance();
        sql::SQLString hostname("tcp://" + credentials.getHostname() + ":" + std::to_string(credentials.getPort()));
		
        if (context != nullptr && context->verboseMode)
            std::cout << "MySQL hostname value being set as: " << hostname << std::endl;

        // If credentials.getPassword is set, this is set for test mode.
        if (credentials.getPassword().empty())
            con = driver->connect(hostname, credentials.getUsername(), credentials::password("database connection"));
		else
            con = driver->connect(hostname, credentials.getUsername(), credentials.getPassword());
		    
        /* Connect to the MySQL test database */
        con->setSchema(database);
    }
    catch (sql::SQLException &e)
    {
        printExceptionInfo(e);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


// CREATE TABLE tableName (elements[0], elements[1], ...);
void Database::addTable(const std::string& tableName, const std::vector<std::string>& elements)
{
    try
    {
        stmt = con->createStatement();
        std::string query = "CREATE TABLE " + tableName + " (";
        for (auto & element : elements)
        {
            query += element + ", ";
        }
        query = query.substr(0,query.size()-2); // Remove the last ", " from the string.
        query += ");";
        res = stmt->executeQuery(query); // Get all the rows.
    }
    catch (sql::SQLException &e)
    {
        printExceptionInfo(e);
    }
}


// INSERT INTO tableName (elements[0], elements[1], ...) VALUES (values[0], values[1], ...);
void Database::addElementToTable(const std::string& tableName, 
                                 const std::vector<std::string>& elements, 
                                 std::vector<std::string> values)
{
    try
    {
        stmt = con->createStatement();
        std::string query = "INSERT INTO " + tableName + " (";
        for (auto & element : elements)
        {
            query += element + ", ";
        }
        query = query.substr(0,query.size()-2); // Remove the last ", " from the string.
        query += ") VALUES (";
        for (auto & value : values)
        {
            query += value + ", ";
        }
        query = query.substr(0,query.size()-2); // Remove the last ", " from the string.
        query += ");";
        res = stmt->executeQuery(query); // Get all the rows.
    }
    catch (sql::SQLException &e)
    {
        printExceptionInfo(e);
    }
}


// SELECT * FROM tableName
std::string Database::viewTable(const std::string& tableName)
{
    std::ostringstream output;
    try
    {
        stmt = con->createStatement();
        res = stmt->executeQuery("SELECT * FROM " + tableName + ";"); // Get all the rows.
        unsigned int columns = res->getMetaData()->getColumnCount();
        // Loop over each row.
        while (res->next())
        {
            // Access column data by numeric offset, 1 is the first column
            for(size_t i=1; i <= columns; i++)
            {
                output << res->getString((uint32_t)i);
                if (i != columns)
                    output << ", ";
            }
            output << '\n';
        }
    }
    catch (sql::SQLException &e)
    {
        printExceptionInfo(e);
    }

    std::string result = output.str();
    std::cout << result;
    return result;
}


void Database::printExceptionInfo(sql::SQLException& e)
{
    if (e.getErrorCode() == 0)
        return;
	
    std::cout << "# ERR: SQLException in " << __FILE__
         << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl
         << "# ERR: " << e.what()
         << " (MySQL error code: " << e.getErrorCode()
         << ", SQLState: " << e.getSQLState() << " )" << std::endl;
}


// DROP TABLE tableName;
void Database::deleteTable(const std::string& tableName)
{
    try
    {
        stmt = con->createStatement();
        res = stmt->executeQuery("DROP TABLE " + tableName + ";");
    }
    catch (sql::SQLException &e)
    {
        printExceptionInfo(e);
    }
}
