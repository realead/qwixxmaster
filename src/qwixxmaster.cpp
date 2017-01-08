
#include <iostream> 
#include <string>
#include <sstream>
#include <vector>


#include "game.h"
#include "StringUtils.h"


struct Control{
   size_t sampling_number;
   std::string mem_file;
   Control():
     sampling_number(2){}
};

Control parse_command_line(int argc, char *argv[]){

   Control res;
   for(int i=1;i<argc;i+=2){
        if(argv[i]==std::string("-s")){
            res.sampling_number=stringutils::str2int(argv[i+1]);
        }
        if(argv[i]==std::string("-m")){
           res.mem_file=argv[i+1];
        }
   }
   return res;
}

int main(int argc, char *argv[]){
    Control control=parse_command_line(argc, argv);
    game g(control.sampling_number, control.mem_file);
    
    std::cout<<"Welcome to qwixxmaster!"<<std::endl;
    
    while(true){
       std::cout<<">>>";
       std::string line;
       if(!std::getline(std::cin, line) ||
          !g.execute_command(stringutils::split(line), std::cout))
            break;
    }
    
    std::cout<<"exiting..."<<std::endl;
}

