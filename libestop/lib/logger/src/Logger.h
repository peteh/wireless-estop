#ifndef LOGGER_H
#define LOGGER_H

class ILogger
{
public:
    virtual void debug(const char *str) = 0;

    virtual void info(const char *str) = 0;

    virtual void warn(const char *str) = 0;

    virtual void error(const char *str) = 0;

    virtual ~ILogger(){};
};

#endif