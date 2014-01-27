#include <ctime>
#include <iostream>

#include <ldns/ldns.h>

#include "DNSResolver.hpp"
#include "Logger.hpp"

using namespace te;
using std::string;
using std::cout;
using std::endl;

DNSResolver::DNSResolver()
{
    srand((unsigned)time(0));
}

bool DNSResolver::resolve(const string& nm, struct timeval &qtime, unsigned& querytime)
{
    ldns_resolver *res = NULL;
    ldns_rdf *name = NULL;
    ldns_pkt *p = NULL;
    ldns_status s ;

    int rndPrefix = rand() % 100000 ;
    char buff[8];
    memset(buff, 0, 8);
    snprintf(buff, 7, "%d.", rndPrefix);

    string prefixedName (string(buff) + nm);
    debug("Prefixed name :%s", prefixedName.c_str());

    name = ldns_dname_new_frm_str(prefixedName.c_str());
    if (!name)
    {
        error("could not create domain name from '%s'" , nm.c_str());
        return false;
    }

    /* create a new resolver from /etc/resolv.conf */
    s = ldns_resolver_new_frm_file(&res, NULL);
    if (s != LDNS_STATUS_OK)
    {
        ldns_rdf_deep_free(name);
        ldns_resolver_deep_free(res);
        error("could not create resolver for '%s'", nm.c_str());
        return false;
    }

    p = ldns_resolver_query(res,
                            name,
                            LDNS_RR_TYPE_A,
                            LDNS_RR_CLASS_IN,
                            LDNS_RD);

    ldns_rdf_deep_free(name);
    ldns_resolver_deep_free(res);

    if(!p)
    {
        error("could not dns reply packet for '%s'", nm.c_str());
        return false;
    }

    debug("resolved with querytime :%u", p->_querytime);

    querytime = p->_querytime;
    qtime = p->timestamp;

    return true;
}
