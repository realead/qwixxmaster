#include "StringUtils.h"

#include <sstream>

std::string stringutils::join(const std::vector<std::string> &strs, const std::string &delimeter){
    std::stringstream res;
    for(size_t i=0;i<strs.size();i++){
      if(i!=0)  
        res<<delimeter;
      res<<strs.at(i);
    }
    return res.str();
}


