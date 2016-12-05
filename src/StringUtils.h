#pragma once

#include <vector>
#include <string>

namespace stringutils{
    std::string join(const std::vector<std::string> &strs, const std::string &delimeter=" ");
    bool str2int(const std::string &str, int &result);
}
