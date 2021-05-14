#include "Log.h"
#include "CoutLogger.h"
#include <cstdio>

Log *Log::m_instance = 0;

void Log::init(ILogger *log)
{
    instance()->m_log = log;
}

void Log::info(const String &str)
{
    instance()->m_log->info(str.c_str());
}

void Log::info(const char *str)
{
    instance()->m_log->info(str);
}

void Log::info(const char *fmt, va_list argp)
{
    char *s = NULL;
    sprintf(s, fmt, argp);
    info(s);
}


void Log::warn(const String &str)
{
    instance()->m_log->warn(str.c_str());
}

void Log::warn(const char *str)
{
    instance()->m_log->warn(str);
}

void Log::warn(const char *fmt, va_list argp)
{
    char *s = NULL;
    sprintf(s, fmt, argp);
    warn(s);
}


void Log::error(const String &str)
{
    instance()->m_log->error(str.c_str());
}

void Log::error(const char *str)
{
    instance()->m_log->error(str);
}

void Log::error(const char *fmt, va_list argp)
{
    char *s = NULL;
    sprintf(s, fmt, argp);
    error(s);
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