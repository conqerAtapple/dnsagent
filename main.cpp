#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include <string>
#include <mysql++/mysql++.h>

#include "Constants.hpp"
#include "DNSPinger.hpp"
#include "Environment.hpp"
#include "Logger.hpp"

using namespace te;

const static int DEFAULT_FREQUENCY_SECS = 10;
const static unsigned MAX_FREQUENCY_SECS = 10 * 86400; // 10 days

const static std::string domains[] = {
    "google.com",
    "facebook.com",
    "youtube.com",
    "yahoo.com",
    "live.com",
    "wikipedia.org",
    "baidu.com",
    "blogger.com",
    "msn.com",
    "qq.com",
};

static bool keepRunning = true;
static bool isStatsOnly = false;
static unsigned pingFrequency = DEFAULT_FREQUENCY_SECS;

void sigHandler(int sig)
{
    keepRunning = false;
}

void usage()
{
    fprintf(stdout, "Usage :dnsagent -d <db name> -u <db user> -p <db password> [-f <ping frequency in secs>][-h host][-s]\n\
            \t\t -d : Database name\n\
            \t\t -u : Database user name\n\
            \t\t -p : Database password\n\
            \t\t -h : Database host (defaults to 'localhost' if not specified)\n\
            \t\t -f : DNS ping frequency\n\
            \t\t -s : only prints summary and skips the ping\n");
}

void start()
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sigHandler;
    sigfillset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

    std::vector<DNSPinger*> dnsPingers;

    try
    {
        for (size_t i=0; i < sizeof(domains)/sizeof(std::string); ++i)
        {
            te::DNSPinger *dp = new te::DNSPinger(domains[i]);
            dnsPingers.push_back(dp);
            dp->init();
        }

        std::vector<DNSPinger*>::const_iterator iEnd = dnsPingers.end();

        if(!isStatsOnly)
        {
            int count = 0;
            while (keepRunning)
            {
                report ("Starting ping # %d for all domains", ++count);
                for (std::vector<DNSPinger*>::const_iterator i = dnsPingers.begin(); i != iEnd; ++i)
                {
                    (*i)->run();
                }
                report ("Finished ping # %d for all domains", count);

                sleep(pingFrequency);
            }

            report("Finished pinging. Printing summary..");
        }

        for (std::vector<DNSPinger*>::iterator i = dnsPingers.begin(); i != iEnd; ++i)
        {
            (*i)->printSummary();
            delete *i;
        }
    }
    catch (std::exception &e)
    {
        error ("caught exception :%s", e.what());
    }
    catch (std::string &e)
    {
        error ("caught string exception :%s", e.c_str());
    }
    catch (...)
    {
        error ("Caught unknown exception");
    }
}

int main(int argc, char **argv)
{
    int c = 0;
    char *dbName = NULL, *dbUser = NULL, *dbPasswd = NULL;
    std::string dbHost, frequency;

    while ((c = getopt(argc, argv, "d:u:p:f:h:s")) != -1)
    {
       switch(c)
       {
       case 'd' :
           {
               dbName = optarg;
               break;
           }
       case 'u':
           {
               dbUser = optarg;
               break;
           }
       case 'p':
           {
               dbPasswd = optarg;
               break;
           }
       case 'h':
           {
               dbHost = (optarg == NULL) ? "": optarg;
               break;
           }
       case 'f':
           {
               frequency = (optarg == NULL) ? "": optarg;
               pingFrequency = atoi(frequency.c_str());
               break;
           }
       case 's':
           {
               isStatsOnly = true;
               break;
           }
       default:
           {
               usage();
               exit(-1);
           }
       }
    }

    if ((dbName == NULL) ||
        (dbUser == NULL) ||
        (dbPasswd == NULL) ||
        (pingFrequency == 0) ||
        (pingFrequency > MAX_FREQUENCY_SECS))
    {
        if (pingFrequency == 0 || pingFrequency > MAX_FREQUENCY_SECS)
        {
            error("Ping frequency should be in range [0..%u] secs", MAX_FREQUENCY_SECS);
        }
        else
        {
            error ("Invalid input parameter: database name: '%s',\
                database user: %s, \
                database password: %s", dbName, dbUser, dbPasswd);
        }

        usage();
        exit(-1);
    }

    Environment::getInstance()->addProperty(Constants::CONST_DB_NAME, dbName);
    Environment::getInstance()->addProperty(Constants::CONST_DB_USER, dbUser);
    Environment::getInstance()->addProperty(Constants::CONST_DB_PASSWORD, dbPasswd);
    Environment::getInstance()->addProperty(Constants::CONST_DB_HOST, dbHost);
    Environment::getInstance()->addProperty(Constants::CONST_PING_FREQUENCY, frequency);

    start();

    return 0;
}
