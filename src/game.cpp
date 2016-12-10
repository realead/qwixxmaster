#include "game.h"

#include <iostream>

#include "StringUtils.h"
#include "QuixxException.h"

#include "Command.h"

namespace{
    
    void set(State &state, std::ostream &out, const game::Command &command){
       //format of set: <taken_red> <last_red> <taken_yellow> <last_yellow> <taken_green> <last_green> <taken_blue> <last_blue> <missed>
        if(command.size()!=10)
            THROW_QUIXX("invalid syntax: '"<<stringutils::join(command)
                        <<"'. Known syntax is 'set <taken_red> <last_red>"
                          " <taken_yellow> <last_yellow> <taken_green> <last_green>"
                          " <taken_blue> <last_blue> <missed>'");       

       State new_state;
       for(size_t i=0;i<COLOR_CNT;i++){
            Color color=static_cast<Color>(i);
            int taken, last;
            if(!stringutils::str2int(command.at(i*2+1), taken) ||
               !stringutils::str2int(command.at(i*2+2), last)){
               out << "cannot parse for color "<<color2str(color)<<std::endl;
               return;   
            }
            if(!new_state.set(color, last, taken)){
               out << "invalid set for color "<<color2str(color)<<std::endl;
            }
       }
       int missed;
       if(!stringutils::str2int(command.back(), missed)){
            out << "cannot parse missed"<<std::endl;
            return; 
       }
       if(!new_state.set_missed(missed)){
            out << "invalid missed"<<std::endl;
            return;       
       }
       
       //everthing ok:
       state=new_state;
    }

}


bool game::execute_command(const std::vector<std::string> &command, std::ostream &out){

    try{
        if(command.empty())
            return true;
                        
        if(command.at(0)=="set"){
            set(state, out, command);
            return true;
        }
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


