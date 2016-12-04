
#include <iostream> 
#include "game.h"

std::vector<std::string> split(const std::string &line){
    std::vector<std::string> res;
    return res;
}


int main(){
    std::cout<<"Welcome to quixxmaster!"<<std::endl;
    game g;
    
    while(true){
       std::cout<<">>>";
       std::string line;
       std::getline(std::cin, line);
       
       if(!g.execute_command(split(line)))
            break;
    }
    
    std::cout<<"\nexiting..."<<std::endl;
}

