#ifndef DNSRESOLVER_HPP
#define DNSRESOLVER_HPP

#include <time.h>
#include <string>

namespace te
{
    class DNSResolver
    {
        public:
            DNSResolver();

            bool resolve(const std::string& , struct timeval&, unsigned&);
    };
}

#endif

