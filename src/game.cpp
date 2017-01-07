#include "game.h"

#include <iostream>

#include "StringUtils.h"
#include "QwixxException.h"

#include "Command.h"



game::game(size_t sampling_number_, const std::string &mem_file): 
      evaluator(sampling_number_)
{
    if(!mem_file.empty()){
       evaluator.load_memory_from_file(mem_file);
    }
}

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
    catch (QwixxException &ex){
      out << ex.what() << std::endl;
    }
    
    
    return true;
}


