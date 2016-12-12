#include "game.h"

#include <iostream>

#include "StringUtils.h"
#include "QuixxException.h"

#include "Command.h"



game::game(size_t sampling_number_): sampling_number(sampling_number_){}

bool game::execute_command(const std::vector<std::string> &command, std::ostream &out){

    if(command.empty()){
          return true;
    }
    
    if(command.at(0)=="evaluate"){
        if(evaluator==NULL)
            evaluator.reset(new Evaluator(sampling_number));
        out<<"Expected score: "<<evaluator->evaluate_state(state)<<std::endl;
        return true;
    }
    
    if(command.at(0)=="roll"){
        if(evaluator==NULL)
            evaluator.reset(new Evaluator(sampling_number));
            
        DiceRoll roll;
        for(size_t i=1;i<=6;i++){
            roll[i-1]=stringutils::str2int(command.at(i));
        }
        Evaluator::MoveInfos infos=evaluator->get_roll_evaluation(state, roll);
        for(const Evaluator::MoveInfo &info:infos)
           out<<info.second<<": "<<info.first<<std::endl;
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


