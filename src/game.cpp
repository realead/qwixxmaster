#include "game.h"

#include <iostream>

#include "StringUtils.h"
#include "QuixxException.h"

#include "Command.h"



game::game(size_t sampling_number_): evaluator(sampling_number_){}

bool game::execute_command(const std::vector<std::string> &command, std::ostream &out){
    
    if(command.empty()){
          return true;
    }
    
    try{
        CommandExecuterPtr executer=CommandDictionary::get_command_executer(command); 
        std::string message=executer->execute(state, evaluator);
        out<<message;
        if(!message.empty())
            out<<std::endl;
        return !executer->exit_program();
    }       
    catch (QuixxException &ex){
      out << ex.what() << std::endl;
    }
    
    
    return true;
}


