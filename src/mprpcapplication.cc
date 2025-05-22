#include "mprpcapplication.h"
#include <iostream>
#include <unistd.h>
#include <string>

MprpcConfig MprpcApplication::m_config;

void ShowArgsHelp() {
    std::cout << "format: command -i <configfile>" << std::endl;
}

void MprpcApplication::Init(int argc, char** argv) {
    if (argc < 2) {
        ShowArgsHelp();
        exit(EXIT_FAILURE);
    }

    int c = 0;
    std::string config_file;
    while ((c = getopt(argc, argv, "i:")) != -1) {
        switch (c) {
            case 'i':
                config_file = optarg;
                break;
            case '?':
                ShowArgsHelp();
                exit(EXIT_FAILURE);
            case ':':
                ShowArgsHelp();
                exit(EXIT_FAILURE);
            default:
                break;
        }
    }

    // Load configuration file rpcserver_ip, rpcserver_port zookeeper_ip zookeeper_port
    m_config.loadConfigFile(config_file.c_str());

    std::cout << "rpcserver_ip: " << m_config.load("rpcserverip") << std::endl;
    std::cout << "rpcserver_port: " << m_config.load("rpcserverport") << std::endl;
    std::cout << "zookeeper_ip: " << m_config.load("zookeeperip") << std::endl;
    std::cout << "zookeeper_port: " << m_config.load("zookeeperport") << std::endl;
}

MprpcApplication& MprpcApplication::GetInstance() {
    static MprpcApplication instance;
    return instance;
}