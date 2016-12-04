
#include <iostream> 
#include "game.h"

std::vector<std::string> split(const std::string &line){
    std::vector<std::string> res;
    if(!line.empty())
        res.push_back(line);
    return res;
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

