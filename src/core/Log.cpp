#include <core/Log.h>

#if defined(__ANDROID__)

#include <cstdarg>

void logf(LogLevel level, const string tag, string format, ...)
{
    va_list args;
    va_start(args, format);
    __android_log_vprint(level, tag.c_str(), format.c_str(), args);
    va_end(args);
}

void Logger::logf(const LogLevel level, string message, ...) const
{
   va_list args;
   va_start(args, message);
   __android_log_vprint(level, tag.c_str(), message.c_str(), args);
   va_end(args);
}

#else

#include <cstdarg>
#include <cstdio>

const string ERROR_PREFIX = string("E/ ");
const string WARN_PREFIX = string("W/ ");
const string INFO_PREFIX = string("I/ ");
const string DEBUG_PREFIX = string("D/ ");

string getLevelPrefix(LogLevel level)
{
    switch (level)
    {
        case LOG_ERROR:
            return ERROR_PREFIX;

        case LOG_WARN:
            return WARN_PREFIX;

        case LOG_INFO:
            return INFO_PREFIX;

        case LOG_DEBUG:
            return DEBUG_PREFIX;
    };

    return "";
}

void Logger::logf(const LogLevel level, string message, ...) const
{
#ifdef NDEBUG
    if (level == LOG_DEBUG)
    {
        return;
    }
#endif


    message += "\n";

    string levelPrefix = getLevelPrefix(level);
    string logPrefix = levelPrefix + tag + " - ";
    fputs(logPrefix.c_str(), stdout);

    va_list args;
    va_start(args, message);
    vprintf(message.c_str(), args);
    va_end(args);
}

#endif

Logger::Logger(const string& tag)
        : tag(tag)
{ }

Logger Logger::create(const string& tag)
{
    return Logger(tag);
}
