
#include <iostream> 
#include "game.h"
#include <string>
#include <sstream>
#include <vector>

std::vector<std::string> split(const std::string &s, char delim=' ') {
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


int main(){
    std::cout<<"Welcome to quixxmaster!"<<std::endl;
    game g;
    
    while(true){
       std::cout<<">>>";
       std::string line;
       std::getline(std::cin, line);
       
       if(!g.execute_command(split(line), std::cout))
            break;
    }
    
    std::cout<<"exiting..."<<std::endl;
}

