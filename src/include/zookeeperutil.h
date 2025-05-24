#pragma once

#include <semaphore.h>
#include <zookeeper/zookeeper.h>
#include <string>

class ZkCLient
{
public:
    ZkClient();

    ~ZkClient();

    // zkclient start zkserver
    void start();

    // create a znode
    void create(const char *path, const char *data, int datalen, int state = 0);

    // based on the path, get the data from znode
    std::string getData(const char *path);

private:
    zhandle_t *m_zhandle;
};