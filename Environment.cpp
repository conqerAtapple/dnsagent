#include "Environment.hpp"
#include "Logger.hpp"

using namespace te;
using std::map;
using std::string;

Environment* Environment::instance = NULL;
map<string, string> Environment::propMap;

Environment::Environment()
{
}

Environment* Environment::getInstance()
{
    if (instance == NULL)
    {
        instance = new Environment();
    }

    return instance;
}

void Environment::addProperty(const string &key, const string &val)
{
    debug("Adding environment property '%s' --> '%s'", key.c_str(), val.c_str());
    propMap.insert(std::make_pair<string, string>(key, val));
}


string Environment::getProperty(const std::string &key)
{
    map<string, string>::const_iterator itr = propMap.find(key);

    if (itr != propMap.end())
    {
        return itr->second;
    }

    return string();
}

void Environment::print()
{
    for ( map<string, string>::const_iterator itr = propMap.begin(); itr != propMap.end(); ++itr)
    {
        info(" Key :'%s', Value :'%s'" , (itr->first).c_str(), (itr->second).c_str()) ;
    }
}
