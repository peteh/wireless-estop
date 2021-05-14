#ifndef LOG_H
#define LOG_H
#include "Logger.h"
#include <stdarg.h>
#include "WString.h"

class Log
{
public:
    static void init(ILogger *log);

    static void info(const String &str);
    static void info(const char *str);
    static void info(const char *fmt, va_list argp);

    static void warn(const String &str);
    static void warn(const char *str);
    static void warn(const char *fmt, va_list argp);

    static void error(const String &str);
    static void error(const char *str);
    static void error(const char *fmt, va_list argp);
    
private:
    Log();
    static Log *instance();
    static Log *m_instance;
    
    ILogger *m_log;
};


#endif