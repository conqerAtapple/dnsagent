#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <string>
#include <vector>

#include <mysql++/mysql++.h>

namespace te
{
    class Database
    {
        public:
            static Database* getInstance();

            ~Database();

            static size_t runQuery(const std::string& queryStr, std::vector<mysqlpp::Row>& result);

        private:
            std::string dbName;
            std::string dbUser;
            std::string dbPasswd;
            std::string dbHost;

            static Database *instance;
            static mysqlpp::Connection connection;
            bool isInit;

            //
            //Methods
            //
            //
            Database();

            void init();
            void createDatabase();
            void setupTables();
    };
}

#endif
