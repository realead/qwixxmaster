#pragma once

#include <stdexcept>
#include <sstream>

class QuixxException: public std::runtime_error{
    public:
        QuixxException(const std::string &message) : std::runtime_error(message){}
};

#define THROW_QUIXX(message)\
{\
    std::stringstream ss;\
    ss<<message;\
    throw QuixxException(ss.str());\
}


