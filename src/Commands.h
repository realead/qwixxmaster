#pragma once

#include "Command.h"

//Score:

class ScoreCommandExecuter: public CommandExecuter{
public: 
     virtual std::string execute(State &state);
     virtual bool exit_program();
};

class ScoreCommandParser: public CommandParser{
public:
    virtual CommandExecuterPtr parse(const CommandLine &line);
    static std::string command_name();
};


//Exit
class ExitCommandExecuter: public CommandExecuter{
public: 
     virtual std::string execute(State &state);
     virtual bool exit_program();
};

class ExitCommandParser: public CommandParser{
public:
    virtual CommandExecuterPtr parse(const CommandLine &line);
    static std::string command_name();
};

