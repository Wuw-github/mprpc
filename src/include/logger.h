#pragma once
#include "lockqueue.h"
#include <string>

// provide the logging structure for the rpc system
enum LogLevel
{
    INFO,
    ERROR,
};

class Logger
{

public:
    static Logger &getInstance();

    void setLogLevel(int level);

    void log(std::string msg);

private:
    Logger();
    Logger(const Logger &) = delete;
    Logger(Logger &&) = delete;

public:
    int m_logLevel;
    LockQueue<std::string> m_logQueue;
};

#define LOG_INFO(logmsgformat, ...)                                    \
    do                                                                 \
    {                                                                  \
        char logmsg[1024];                                             \
        snprintf(logmsg, sizeof(logmsg), logmsgformat, ##__VA_ARGS__); \
        Logger &logger = Logger::getInstance();                        \
        logger.setLogLevel(INFO);                                      \
        logger.log(std::string(logmsg));                               \
    } while (0);

#define LOG_ERROR(logmsgformat, ...)                                   \
    do                                                                 \
    {                                                                  \
        char logmsg[1024];                                             \
        snprintf(logmsg, sizeof(logmsg), logmsgformat, ##__VA_ARGS__); \
        Logger &logger = Logger::getInstance();                        \
        logger.setLogLevel(ERROR);                                     \
        logger.log(std::string(logmsg));                               \
    } while (0);