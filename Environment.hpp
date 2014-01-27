#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <map>
#include <string>

namespace te
{
    class Environment
    {
        public:
            static Environment* getInstance();
            static void addProperty(const std::string &key, const std::string &val);
            static std::string getProperty(const std::string &key);

            static void print();

        private:
            static Environment* instance;
            static std::map<std::string, std::string> propMap;

            //
            //Methods
            //
            Environment();
    };
}

#endif
