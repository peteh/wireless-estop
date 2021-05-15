#ifndef LOG_H
#define LOG_H
#include "Logger.h"
#include <stdarg.h>

class Log
{
public:
    static void init(ILogger *log);

    static void debug(const char *str);
    static void debugf(const char *fmt, ...);

    static void info(const char *str);
    static void infof(const char *fmt, ...);

    static void warn(const char *str);
    static void warnf(const char *fmt, ...);

    static void error(const char *str);
    static void errorf(const char *fmt, ...);

private:
    Log();
    static Log *instance();
    static Log *m_instance;
    
    ILogger *m_log;
};


#endif