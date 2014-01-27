#ifndef DOMAIN_HPP
#define DOMAIN_HPP

#include <string>

namespace te
{
    class Domain
    {
        public:
            Domain(const std::string &name);

            int getId() const
            {
                return uniqid;
            }

            const std::string& getName() const
            {
                return name;
            }

            void load();

        private:
            std::string name;
            int uniqid;
    };
}

#endif
