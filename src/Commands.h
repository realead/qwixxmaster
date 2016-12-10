#pragma once

#include "Command.h"


#define CREATE_COMMAND_DECLARATION(Name)\
class  Name##CommandExecuter: public CommandExecuter{\
public: \
     virtual std::string execute(State &state);\
     virtual bool exit_program();\
};\
class Name##CommandParser: public CommandParser{\
public:\
    virtual CommandExecuterPtr parse(const CommandLine &line);\
    static std::string command_name();\
};

CREATE_COMMAND_DECLARATION(Score);
CREATE_COMMAND_DECLARATION(Exit);
CREATE_COMMAND_DECLARATION(Print);

