#include "StringUtils.h"

#include <sstream>
#include <stdexcept>

std::string stringutils::join(const std::vector<std::string> &strs, const std::string &delimeter){
    std::stringstream res;
    for(size_t i=0;i<strs.size();i++){
      if(i!=0)  
        res<<delimeter;
      res<<strs.at(i);
    }
    return res.str();
}


bool stringutils::str2int(const std::string &str, int &result){
    size_t processed=0;   
    try{
        result=stoi(str, &processed);
    }
    catch(std::invalid_argument &){
        return false;
    }
    catch(std::out_of_range &){
        return false;
    }
    return processed==str.size();
}

std::string stringutils::int2str(int value){
    std::stringstream res;
    res<<value;
    return res.str();
}



std::vector<std::string> stringutils::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        if(!item.empty())
            elems.push_back(item);
    }
    return elems;
}


