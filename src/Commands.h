#pragma once

#include "Command.h"


class ScoreCommand: public Command{
public: 
     virtual std::string check_syntax(const CommandLine &line);
     virtual std::string check_validity(const State &state, const CommandLine &line);
     virtual std::string execute(State &state, const CommandLine &line);  
     

     virtual bool exit_program();
     
     
     
     static std::string command_name();
};


