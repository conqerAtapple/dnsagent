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

const size_t Domain::MAX_DOMAIN_LENGTH = 500;

Domain::Domain(const string &nm):
    name(nm),
    uniqid(-1)
{
    load();
}

void Domain::load()
{
    if(name.length() > MAX_DOMAIN_LENGTH)
    {
        throw string ("Domain name exceeds max allowed length");
    }

    Database *db = Database::getInstance();
    char query[MAX_DOMAIN_LENGTH + 100];

    snprintf(query, MAX_DOMAIN_LENGTH + 100, "INSERT IGNORE INTO domains VALUES ('', '%s');", name.c_str());
    vector<Row> results;
    db->runQuery(query, results);

    results.clear();
    snprintf(query, MAX_DOMAIN_LENGTH + 100, "SELECT * FROM domains WHERE name='%s';", name.c_str());
    if (db->runQuery(query, results))
    {
        uniqid = results[0][0];
    }
    else
    {
        throw string("Unable to get unique id from DB");
    }

}

