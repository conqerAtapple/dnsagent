#include <iostream>
#include <vector>

#include <stdio.h>
#include <sys/time.h>

#include "Constants.hpp"
#include "DNSPinger.hpp"
#include "Environment.hpp"
#include "Logger.hpp"

#define QTIME_INFINITE ~0L

using namespace mysqlpp;
using namespace te;
using std::string;
using std::vector;
using std::cout;
using std::endl;

DNSPinger::DNSPinger(const string& nm):
    name(nm),
    domain(nm),
    ping(domain)
{
}

void DNSPinger::init()
{
    debug("Initializing pinger for '%s'", name.c_str());
    string strFreq = Environment::getInstance()->getProperty(Constants::CONST_PING_FREQUENCY);
    frequency = atoi (strFreq.c_str());
}

void DNSPinger::run()
{
    unsigned qtime=0;
    struct timeval rtime;

    memset(&rtime, 0, sizeof(rtime));

    if(resolver.resolve(name, rtime, qtime))
    {
        debug("Request time secs :%lu , msecs: %lu", rtime.tv_sec,rtime.tv_usec);
        ping.update(rtime, qtime);
    }
    else
    {
        gettimeofday(&rtime, NULL);
        ping.update(rtime, QTIME_INFINITE);
    }
}


void DNSPinger::printSummary() const
{
    ping.printSummary();
}
