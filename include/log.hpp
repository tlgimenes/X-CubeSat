/*
 * CLASS LOG : This class deals with the log file and
 * errors
 *
 */

#ifndef LOG_HPP
#define LOG_HPP

#include <fstream>
#include <string>

#define LOG_FILE "XCubeSat_Controler.txt"

typedef enum logLevel_t
{
    LEVEL_LOG_ERROR,
    LEVEL_LOG_WARNING,
    LEVEL_LOG_INFO
} logLevel;

class Log
{
    private:
        static std::fstream *logFile;

    public:
        static void LogWarn(logLevel, const char*, const char*, int);
};

#endif
