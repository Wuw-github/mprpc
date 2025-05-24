#include "zookeeperutil.h"
#include "mprpcapplication.h"
#include <semaphore.h>
#include <iostream>

// global watcher for zookeeper
void global_watcher(zhandle_t *zh, int type, int state, const char *path)
{
    if (type == ZOO_SESSION_EVENT) // callback for message event
    {
        if (state == ZOO_CONNECTED_STATE) // connected to zookeeper
        {
            sem_t *sem = static_cast<sem_t *>(zoo_get_context(zh));
            sem_post(sem); // post the semaphore to notify the main thread
        }
    }
}

ZkClient::ZkClient()
    : m_zhandle(nullptr)
{
}

~ZkClient::ZkClient()
{
    if (m_zhandle)
    {
        zookeeper_close(m_zhandle);
    }
}

void ZkClient::start()
{
    std::string host = MprpcApplication::GetConfig().load("zookeeperip");
    int port = std::stoi(MprpcApplication::GetConfig().load("zookeeperport"));
    std::string conn_str = host + ":" + std::to_string(port);

    /*
        zookeeper_mt: multi-threaded zookeeper client
        three threads:
        1. main thread
        2. network IO thread
        3. watcher callback thread
        zookeeper_init: async operation
    */
    m_zhandle = zookeeper_init(conn_str.c_str(), global_watcher, 30000, nullptr, nullptr, 0);

    // if set up the zookeeper failed
    if (m_zhandle == nullptr)
    {
        std::cerr << "Zookeeper initialization failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    semt_t sem;
    sem_init(&sem, 0, 0);
    zoo_set_context(m_zhandle, &sem);

    sem_wait(&sem);

    std::cout << "Zookeeper client started" << std::endl;
}

void ZkClient::create(const char *path, const char *data, int datalen, int state)
{
    char path_buffer[128] = {0};
    int bufferlen = sizeof(path_buffer);
    int flag = 0;
    flag = zoo_exists(m_zhandle, path, 0, nullptr);
    if (flag == ZNONODE)
    {
        flag = zoo_create(m_zhandle, path, data, datalen, &ZOO_OPEN_ACL_UNSAFE, state, path_buffer, bufferlen);

        if (flag == ZOK)
        {
            std::cout << "Znode created: " << path << std::endl;
        }
        else
        {
            std::cerr << "Znode creation failed: " << path << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

std::string ZKClient::getData(const char *path)
{
    char buffer[128] = {0};
    int bufferlen = sizeof(buffer);
    int flag = zoo_get(m_zhandle, path, 0, buffer, &bufferlen, nullptr);

    if (flag == ZOK)
    {
        std::cout << "Znode data: " << buffer << std::endl;
    }
    else
    {
        std::cerr << "Failed to get znode data: " << path << std::endl;
        return "";
    }
}