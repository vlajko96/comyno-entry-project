
#ifndef _CM_LOG_H_
#define _CM_LOG_H_

#include <cstring>
#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <ctime>

#ifdef LOGGING
    #define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

    #define MAX_LOG_LENGTH 500

    #define VERBOSE_LOG(...) {char log_string[MAX_LOG_LENGTH] = {'\0'};\
                                    if (LOG_LEVEL <= 0) {snprintf(log_string, MAX_LOG_LENGTH, __VA_ARGS__);\
                                    std::time_t t = std::time(nullptr);\
                                    char buff[50];\
                                    std::strftime(buff, sizeof(buff), "%b %d %Y %H:%M:%S", std::localtime(&t));\
                                    std::cout << "[ " << buff << " ] [ VERBOSE ] [ "\
                                    <<__FILENAME__<< ":" <<__LINE__<< " ] " << log_string << std::endl;}}
    #define DEBUG_LOG(...)   {char log_string[MAX_LOG_LENGTH] = {'\0'};\
                                    if (LOG_LEVEL <= 1) {snprintf(log_string, MAX_LOG_LENGTH, __VA_ARGS__);\
                                    std::time_t t = std::time(nullptr);\
                                    char buff[50];\
                                    std::strftime(buff, sizeof(buff), "%b %d %Y %H:%M:%S", std::localtime(&t));\
                                    std::cout << "[ " << buff << " ] [ DEBUG ] [ "\
                                    << __FILENAME__ << ":" << __LINE__ << " ] " << log_string << std::endl;}}
    #define INFO_LOG(...)    {char log_string[MAX_LOG_LENGTH] = {'\0'};\
                                    if (LOG_LEVEL <= 2) {snprintf(log_string, MAX_LOG_LENGTH, __VA_ARGS__);\
                                    std::time_t t = std::time(nullptr);\
                                    char buff[50];\
                                    std::strftime(buff, sizeof(buff), "%b %d %Y %H:%M:%S", std::localtime(&t));\
                                    std::cout << "[ " << buff << " ] [ INFO ] [ "\
                                    <<__FILENAME__<< ":" <<__LINE__<< " ] " << log_string << std::endl;}}
    #define NOTICE_LOG(...)  {char log_string[MAX_LOG_LENGTH] = {'\0'};\
                                    if (LOG_LEVEL <= 3) {snprintf(log_string, MAX_LOG_LENGTH, __VA_ARGS__);\
                                    std::time_t t = std::time(nullptr);\
                                    char buff[50];\
                                    std::strftime(buff, sizeof(buff), "%b %d %Y %H:%M:%S", std::localtime(&t));\
                                    std::cout << "[ " << buff << " ] [ NOTICE ] [ "\
                                    << __FILENAME__ << ":" << __LINE__ << " ] " << log_string << std::endl;}}
    #define WARNING_LOG(...) {char log_string[MAX_LOG_LENGTH] = {'\0'};\
                                    if (LOG_LEVEL <= 4) {snprintf(log_string, MAX_LOG_LENGTH, __VA_ARGS__);\
                                    std::time_t t = std::time(nullptr);\
                                    char buff[50];\
                                    std::strftime(buff, sizeof(buff), "%b %d %Y %H:%M:%S", std::localtime(&t));\
                                    std::cout << "[ " << buff << " ] [ WARNING ] [ "\
                                    <<__FILENAME__<< ":" <<__LINE__<< " ] " << log_string << std::endl;}}
    #define ERROR_LOG(...)   {char log_string[MAX_LOG_LENGTH] = {'\0'};\
                                    if (LOG_LEVEL <= 5) {snprintf(log_string, MAX_LOG_LENGTH, __VA_ARGS__);\
                                    std::time_t t = std::time(nullptr);\
                                    char buff[50];\
                                    std::strftime(buff, sizeof(buff), "%b %d %Y %H:%M:%S", std::localtime(&t));\
                                    std::cout << "[ " << buff << " ] [ ERROR ] [ "\
                                    << __FILENAME__ << ":" << __LINE__ << " ] " << log_string << std::endl;}}
    #define FATAL_LOG(...)   {char log_string[MAX_LOG_LENGTH] = {'\0'};\
                                    if (LOG_LEVEL <= 6) {snprintf(log_string, MAX_LOG_LENGTH, __VA_ARGS__);\
                                    std::time_t t = std::time(nullptr);\
                                    char buff[50];\
                                    std::strftime(buff, sizeof(buff), "%b %d %Y %H:%M:%S", std::localtime(&t));\
                                    std::cout << "[ " << buff << " ] [ FATAL ] [ "\
                                    << __FILENAME__ << ":" << __LINE__ << " ] " << log_string << std::endl;}}

#else

#define DEBUG_LOG(...)
#define INFO_LOG(...)
#define NOTICE_LOG(...)
#define WARNING_LOG(...)
#define ERROR_LOG(...)
#define FATAL_LOG(...)

#endif

#endif  /* _CM_LOG_H_ */