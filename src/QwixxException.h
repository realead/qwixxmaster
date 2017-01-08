#pragma once

#include <stdexcept>
#include <sstream>

class QwixxException: public std::runtime_error{
    public:
        QwixxException(const std::string &message) : std::runtime_error(message){}
};

#define THROW_QWIXX(message)\
{\
    std::stringstream ss;\
    ss<<message;\
    throw QwixxException(ss.str());\
}


