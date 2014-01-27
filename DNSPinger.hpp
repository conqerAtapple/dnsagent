#ifndef DNSPINGER_HPP
#define DNSPINGER_HPP

#include <string>

#include "Database.hpp"
#include "Domain.hpp"
#include "DNSPing.hpp"
#include "DNSResolver.hpp"

namespace te
{
    class DNSPinger
    {
        public:
            DNSPinger(const std::string& name);

            void init();
            void run();
            void printSummary() const;

        private:
            DNSResolver resolver;
            const std::string name;
            const Domain domain;
            DNSPing ping;
            unsigned frequency;

            //
            // Methods
            //
            DNSPinger(const DNSPinger&);
    };
}

#endif
