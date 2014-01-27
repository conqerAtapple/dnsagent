#include <vector>
#include <assert.h>

#include "Constants.hpp"
#include "Database.hpp"
#include "Environment.hpp"
#include "Logger.hpp"

using namespace mysqlpp;
using namespace std;
using namespace te;

const static string tables[] = {
        "domains",
        "pings",
        "pingstats"
    };


Database* Database::instance = NULL;
Connection Database::connection;

Database::Database()
    :isInit(false)
{
    init();
}

Database::~Database()
{
    if (instance)
    {
        delete instance;
    }

    connection.disconnect();
}

Database* Database::getInstance()
{
    if (instance == NULL)
    {
        instance = new Database();
    }

    return instance;
}

void Database::init()
{
    if (isInit)
    {
        return;
    }

    dbName = Environment::getInstance()->getProperty(Constants::CONST_DB_NAME);
    dbUser = Environment::getInstance()->getProperty(Constants::CONST_DB_USER);
    dbPasswd = Environment::getInstance()->getProperty(Constants::CONST_DB_PASSWORD);
    dbHost = Environment::getInstance()->getProperty(Constants::CONST_DB_HOST);

    dbHost = (dbHost == "")? "localhost" : dbHost;

    createDatabase();

    if((!dbUser.length()) || (!dbPasswd.length()))
    {
        throw string ("Database user name or password is blank");
    }

    connection.connect(dbName.c_str(), dbHost.c_str(), dbUser.c_str(), dbPasswd.c_str());
    setupTables();

    isInit = true;
}

void Database::createDatabase()
{
    if (!dbName.length())
    {
        throw string ("Database name is blank");
    }

    connection.connect("mysql", dbHost.c_str(), dbUser.c_str(), dbPasswd.c_str());
    string query ("CREATE DATABASE IF NOT EXISTS ");
    query += dbName;

    vector<Row> results;
    runQuery(query, results);
    connection.disconnect();
}


size_t Database::runQuery(const string& queryStr, vector<Row>& result)
{

    debug("Running query :'%s'",queryStr.c_str());
    Query query = connection.query();
    query << queryStr;
    query.storein(result);

    return result.size();
}

void Database::setupTables()
{

    vector<Row> rows;

    {
        string queryStr ("CREATE TABLE IF NOT EXISTS domains (\
            id MEDIUMINT PRIMARY KEY AUTO_INCREMENT, \
            name VARCHAR(500) UNIQUE NOT NULL)"
            );

        runQuery(queryStr, rows);
    }

    {
        rows.clear();
        {
            string queryStr ("CREATE TABLE IF NOT EXISTS pings (\
                id MEDIUMINT, \
                request BIGINT , \
                timetaken BIGINT, \
                FOREIGN KEY (id) REFERENCES domains(id),\
                PRIMARY KEY (id, request))"
                );

            runQuery(queryStr, rows);
        }
    }
}
