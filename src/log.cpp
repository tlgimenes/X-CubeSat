/*
 * CLASS LOG : This class deals with the log file and
 * errors
 *
 */

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <errno.h>   /* Error number definitions */

#include "log.hpp"

std::fstream * Log::logFile = new std::fstream(LOG_FILE, std::fstream::in | std::fstream::out | std::fstream::ate);

#define writeMessageLogFile(logStr, logMessage, file, line) \
    *(Log::logFile) << logStr; \
    if(file != NULL) *(Log::logFile) << "in file " << file; \
    if(line >= 0) *(Log::logFile) << " at the line " << line << ": "; \
    *(Log::logFile) << logMessage << std::endl;
 
#define writeMessageTerminal(logStr, logMessage, file, line) \
    std::cout  << logStr; \
    if(file != NULL) std::cout << "in file " << file; \
    if(line >= 0) std::cout  << " at the line " << line << ": "; \
    std::cout  << logMessage << std::endl;

void Log::LogWarn(logLevel level, const char* logMessage, const char* file, int line)
{
    switch(level){
        case LEVEL_LOG_ERROR:
            writeMessageLogFile("ERROR: " << strerror(errno) << "! ", logMessage,file, line);
            writeMessageTerminal("ERROR: " << strerror(errno) << "! ", logMessage,file, line);
            exit(1);
            break;
        case LEVEL_LOG_WARNING:
            writeMessageLogFile("WARNING " << strerror(errno) << "! ", logMessage, file, line);
            writeMessageTerminal("WARNING " << strerror(errno) << "! ", logMessage, file, line);
            break;
        case LEVEL_LOG_INFO:
            writeMessageLogFile("INFO ", logMessage, file, line);
            writeMessageTerminal("INFO ", logMessage, file, line);
            break;
        default:
            *(Log::logFile) << "Error message not found ! This incident will be reported" << std::endl;
            exit(1);
    }
}
