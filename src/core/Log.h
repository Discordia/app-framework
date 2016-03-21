#pragma once

#include <string>
using std::string;

#if defined(__ANDROID__)

#include <android/log.h>

enum LogLevel
{
    LOG_ERROR = ANDROID_LOG_ERROR,
    LOG_WARN = ANDROID_LOG_WARN,
    LOG_INFO = ANDROID_LOG_INFO,
    LOG_DEBUG = ANDROID_LOG_DEBUG
};

#else

enum LogLevel
{
    LOG_ERROR,
    LOG_WARN,
    LOG_INFO,
    LOG_DEBUG
};

#endif

class Logger
{
public:
    explicit Logger(const string& tag);

    static Logger create(const string& tag);
    void logf(const LogLevel level, string message, ...) const;

private:

    //!
    const string tag;
};