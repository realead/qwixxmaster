#pragma once

#include "Command.h"

#define CREATE_COMMAND_PARSER_DECLARATION(Name)\
    class Name##CommandParser: public CommandParser{\
    public:\
        virtual CommandExecuterPtr parse(const CommandLine &line);\
        static std::string command_name();\
    };
    
#define CREATE_COMMAND_EXECUTER_DECLARATION(Name)\
    class  Name##CommandExecuter: public CommandExecuter{\
    public: \
         virtual std::string execute(State &state);\
         virtual bool exit_program();\
    };
    
#define CREATE_COMMAND_DECLARATION(Name)\
    CREATE_COMMAND_EXECUTER_DECLARATION(Name);\
    CREATE_COMMAND_PARSER_DECLARATION(Name);

CREATE_COMMAND_DECLARATION(Score);
CREATE_COMMAND_DECLARATION(Exit);
CREATE_COMMAND_DECLARATION(Print);

