#include "mprpcconfig.h"
#include <fstream>
#include <string>
#include <iostream>

void MprpcConfig::loadConfigFile(const char *config_file)
{
    FILE *pf = fopen(config_file, "r");
    if (pf == nullptr)
    {
        std::cout << config_file << " is invalid!" << std::endl;
        exit(EXIT_FAILURE);
    }

    // read the configuration file
    // 1. comments
    // 2. key=value
    // 3. trim
    while (!feof(pf))
    {
        char buf[512] = {0};
        fgets(buf, 512, pf);

        // trim empty space
        std::string line(buf);
        trim(line);

        // check if it is comment or empty line
        if (line[0] == '#' || line.empty())
        {
            continue;
        }

        // key=value
        int idx = line.find('=');
        if (idx == -1)
        {
            std::cout << "Invalid config: " << line << std::endl;
            continue;
        }

        std::string key = line.substr(0, idx);
        trim(key);

        int endIdx = line.find('\n', idx);
        std::string value = line.substr(idx + 1, endIdx - idx - 1);
        trim(value);

        m_configMap.insert({key, value});
    }
}

void MprpcConfig::trim(std::string &str)
{
    // trim left
    int idx = str.find_first_not_of(' ');
    if (idx != -1)
    {
        str = str.substr(idx, str.size() - idx);
    }

    // trim right
    idx = str.find_last_not_of(' ');
    if (idx != -1)
    {
        str = str.substr(0, idx + 1);
    }
}

std::string MprpcConfig::load(const std::string &key)
{
    auto it = m_configMap.find(key);
    if (it == m_configMap.end())
    {
        std::cout << "Key: " << key << " not found!" << std::endl;
        return "";
    }
    return it->second;
}