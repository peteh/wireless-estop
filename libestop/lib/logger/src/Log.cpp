#include "Log.h"
#include "CoutLogger.h"
#include <cstdio>

Log *Log::m_instance = nullptr;

void Log::init(ILogger *log)
{
    instance()->m_log = log;
}

void Log::debug(const char *str)
{
    instance()->m_log->debug(str);
}

void Log::debugf(const char *fmt, ...)
{
    char buffer[256];
    va_list myargs;
    va_start(myargs, fmt);
    vsprintf(buffer, fmt, myargs);
    va_end(myargs);
    debug(buffer);
}

void Log::info(const char *str)
{
    instance()->m_log->info(str);
}

void Log::infof(const char *fmt, ...)
{
    char buffer[256];
    va_list myargs;
    va_start(myargs, fmt);
    vsprintf(buffer, fmt, myargs);
    va_end(myargs);
    info(buffer);
}

void Log::warn(const char *str)
{
    instance()->m_log->warn(str);
}

void Log::warnf(const char *fmt, ...)
{
    char buffer[256];
    va_list myargs;
    va_start(myargs, fmt);
    vsprintf(buffer, fmt, myargs);
    va_end(myargs);
    warn(buffer);
}

void Log::error(const char *str)
{
    instance()->m_log->error(str);
}

void Log::errorf(const char *fmt, ...)
{
    char buffer[256];
    va_list myargs;
    va_start(myargs, fmt);
    vsprintf(buffer, fmt, myargs);
    va_end(myargs);
    error(buffer);
}

Log *Log::instance()
{
    if (!m_instance)
    {
        m_instance = new Log();
    }
    return m_instance;
}

Log::Log()
    : m_log(new CoutLogger())
{
}