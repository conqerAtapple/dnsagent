#include <vector>

#include "Database.hpp"
#include "DNSPing.hpp"
#include "Logger.hpp"

using std::string;
using std::vector;
using namespace te;
using namespace mysqlpp;

DNSPing::DNSPing(const Domain &domain):
    domain(domain)
{
}

void DNSPing::update(struct timeval &rtime, unsigned qtime)
{
    unsigned long sec = rtime.tv_sec;
    debug("Adding timestamp '%lu' for '%s' ", sec, domain.getName().c_str());

    char buff[500] = {0};
    snprintf(buff, 500, "INSERT INTO pings values (%d, %lu, %u);", domain.getId(), sec, qtime);

    vector<Row> results;
    Database::getInstance()->runQuery(buff, results);
}

string getTimeString(const time_t &timeT)
{
    struct tm *timeTm = localtime(&timeT);
    char tmbuf[64];
    strftime(tmbuf, sizeof tmbuf, "%Y-%m-%d %H:%M:%S", timeTm);

    return string(tmbuf);
}

void DNSPing::printSummary() const
{
    string queryStr ("SELECT d.id, d.name, MIN(p.request), MAX(p.request), AVG(p.timetaken), STDDEV(p.timetaken), COUNT(d.id)  FROM \
                        pings AS p,domains AS d WHERE p.id = d.id AND d.name='");
    queryStr += domain.getName().c_str();
    queryStr += "' GROUP BY p.id";

    report("\n\n\t\t|------- Summary for '%s' --------|", domain.getName().c_str());
    vector<Row> results;
    if(! Database::getInstance()->runQuery(queryStr, results))
    {
        report ("\t\t No data available !");
    }
    else
    {
        string startTime = getTimeString(results[0][2]);
        string endTime = getTimeString(results[0][3]);

        report ("\t\t First request time :%s", startTime.c_str());
        report ("\t\t Last request time :%s", endTime.c_str());
        report ("\t\t Average response time :%.3f microseconds", (double)results[0][4]);
        report ("\t\t Standard deviation of response time :%.3f microseconds", (double)results[0][5]);
        report ("\t\t Number of requests made :%d", (int)results[0][6]);
    }

    report("\t\t|---------------------|");
}
