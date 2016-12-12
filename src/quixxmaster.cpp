
#include <iostream> 
#include <string>
#include <sstream>
#include <vector>


#include "game.h"
#include "StringUtils.h"




int main(){
    std::cout<<"Welcome to quixxmaster!"<<std::endl;
    game g(2);
    
    while(true){
       std::cout<<">>>";
       std::string line;
       std::getline(std::cin, line);
       
       if(!g.execute_command(stringutils::split(line), std::cout))
            break;
    }
    
    std::cout<<"exiting..."<<std::endl;
}

