#include <vector>
#include <sys/time.h>

#include "Constants.hpp"
#include "Database.hpp"
#include "Domain.hpp"
#include "Logger.hpp"

using std::string;
using std::vector;
using namespace mysqlpp;
using namespace te;

Domain::Domain(const string &nm):
    name(nm),
    uniqid(-1)
{
    load();
}

void Domain::load()
{
    Database *db = Database::getInstance();
    char query[500];

    snprintf(query, 499, "INSERT IGNORE INTO domains VALUES ('', '%s');", name.c_str());
    vector<Row> results;
    db->runQuery(query, results);

    results.clear();
    snprintf(query, 499, "SELECT * FROM domains WHERE name='%s';", name.c_str());
    if (db->runQuery(query, results))
    {
        uniqid = results[0][0];
    }
    else
    {
        throw string("Unable to get unique id from DB");
    }

}

