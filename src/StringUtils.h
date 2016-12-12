#pragma once

#include <vector>
#include <string>

namespace stringutils{
    std::string join(const std::vector<std::string> &strs, const std::string &delimeter=" ");
    std::vector<std::string> split(const std::string &s, char delim=' ');
    
    bool str2int(const std::string &str, int &result);
    std::string int2str(int value);
}
