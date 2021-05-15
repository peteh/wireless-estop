#ifndef COUT_LOGGER_H
#define COUT_LOGGER_H
#include <iostream>
#include "Logger.h"

class CoutLogger : public ILogger
{
    void debug(const char *str) override
    {
        std::cout << "[DEBUG] "<< str << std::endl;
    }

    void info(const char *str) override
    {
        std::cout << "[INFO] "<< str << std::endl;
    }

    void warn(const char *str) override
    {
        std::cout << "[WARN] " << str << std::endl;
    }

    void error(const char *str) override
    {
        std::cerr << "[ERROR] " << str << std::endl;
    }
    
};

#endif