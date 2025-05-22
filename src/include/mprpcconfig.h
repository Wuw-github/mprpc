#pragma once
#include <unordered_map>
#include <string>

// config file
class MprpcConfig
{

public:
    void loadConfigFile(const char *config_file);
    // query the configuration
    std::string load(const std::string &key);

private:
    void trim(std::string &str);

private:
    std::unordered_map<std::string, std::string> m_configMap;
};