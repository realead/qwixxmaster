#include "game.h"

#include <iostream>

#include "StringUtils.h"
#include "QuixxException.h"

#include "Command.h"


bool game::execute_command(const std::vector<std::string> &command, std::ostream &out){

    try{
        if(command.empty())
            return true;
    }
    catch (QuixxException &ex){
      out << ex.what() << std::endl;
      return true;
    }
    
    try{
        CommandExecuterPtr executer=CommandDictionary::get_command_executer(command); 
        std::string message=executer->execute(state);
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


