#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>

// Basic logging interface.
class Logger {
public:
    static void info(const std::string& msg);
};

#endif // LOGGER_HPP
