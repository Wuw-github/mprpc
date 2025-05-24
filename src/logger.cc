#include "logger.h"
#include <time.h>

Logger::Logger()
{
    m_logLevel = INFO;
    std::thread writeLogTask([&]()
                             {
        for (;;) {
            // get the log message from the queue, get the time, and write to the log file
            time_t now = time(nullptr);
            tm* nowtm = localtime(&now);

            char file_name[128];

            sprintf(file_name, "%d-%d-%d-log.txt", nowtm->tm_year + 1900, nowtm->tm_mon + 1, nowtm->tm_mday);

            FILE* pf= fopen(file_name, "a+");
            if (pf == nullptr) {
                perror("open log file error");
                exit(EXIT_FAILURE);
            }

            char time_buf[128] = {0};
            sprintf(time_buf, "%d:%d:%d", nowtm->tm_hour, nowtm->tm_min, nowtm->tm_sec);
            
            std::string msg = m_logQueue.pop();

            fputs(msg.c_str(), pf);
            fclose(pf);
        } });

    writeLogTask.detach();
}

Logger &Logger::getInstance()
{
    static Logger instance;
    return instance;
}

void Logger::setLogLevel(int level)
{
    m_logLevel = level;
}

void Logger::log(std::string msg)
{
    m_logQueue.push(msg);
}
