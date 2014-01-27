#ifndef DNSPING_HPP
#define DNSPING_HPP

#include <sys/time.h>
#include <string>

#include "Domain.hpp"

namespace te
{
    class DNSPing
    {
        public:
            DNSPing(const Domain&);

            void update(struct timeval &rtime, unsigned qtime);
            void store();
            void printSummary() const;

        private:
            const Domain domain;
    };
}

#endif
