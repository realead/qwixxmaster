#pragma once

#include <array>
#include <string>

#include "Command.h"
#include "State.h"
#include "Evaluator.h"

#define CREATE_COMMAND_PARSER_DECLARATION(Name)\
    class Name##CommandParser: public CommandParser{\
    private:\
        virtual CommandExecuterPtr parse_inner(const CommandLine &line);\
        virtual std::vector<size_t> possible_argument_cnt() const;\
    public:\
        virtual std::string usage() const; \
        virtual std::string description() const; \
        virtual std::string command_name() const; \
    };
    
#define CREATE_COMMAND_EXECUTER_DECLARATION(Name)\
    class  Name##CommandExecuter: public CommandExecuter{\
    public: \
         virtual std::string execute(State &state, Evaluator &evaluator);\
    };
    
#define CREATE_COMMAND_DECLARATION(Name)\
    CREATE_COMMAND_EXECUTER_DECLARATION(Name);\
    CREATE_COMMAND_PARSER_DECLARATION(Name);


CREATE_COMMAND_DECLARATION(Score);
CREATE_COMMAND_DECLARATION(Print);


CREATE_COMMAND_PARSER_DECLARATION(Exit);
class  ExitCommandExecuter: public CommandExecuter{
public: 
     virtual std::string execute(State &state, Evaluator &evaluator);
     virtual bool exit_program() const;
};

CREATE_COMMAND_PARSER_DECLARATION(Take);
CREATE_COMMAND_EXECUTER_DECLARATION(TakeMiss);

class  TakeColorCommandExecuter: public CommandExecuter{
     Color color;
     int number;
public: 
     TakeColorCommandExecuter(Color color, int number);
     virtual std::string execute(State &state,  Evaluator &evaluator);
};

CREATE_COMMAND_PARSER_DECLARATION(Set);


class  SetCommandExecuter: public CommandExecuter{
     std::array<int,COLOR_CNT> last;
     std::array<int,COLOR_CNT> taken;
     int missed;
public: 
     SetCommandExecuter(const std::array<int,COLOR_CNT> &last, const std::array<int,COLOR_CNT> &taken, int missed);
     virtual std::string execute(State &state,  Evaluator &evaluator);
};


CREATE_COMMAND_PARSER_DECLARATION(Possible);

class PossibleCommandExecuter: public CommandExecuter{
     Color color;
     int number;
public: 
     PossibleCommandExecuter(Color color, int number);
     virtual std::string execute(State &state, Evaluator &evaluator);
};


CREATE_COMMAND_DECLARATION(Ended);

CREATE_COMMAND_DECLARATION(Restart);


//Evaluation:

CREATE_COMMAND_PARSER_DECLARATION(Evaluate);

class EvaluateCommandExecuter: public CommandExecuter{
     size_t current_player;
public: 
     EvaluateCommandExecuter(size_t current_player);
     virtual std::string execute(State &state, Evaluator &evaluator);
};

//ROLL:
CREATE_COMMAND_PARSER_DECLARATION(Roll);

template <size_t  len>
class RollCommandExecuter: public CommandExecuter{
     std::array<int, len> roll;
     size_t current_player;
public: 
     RollCommandExecuter(const std::array<int, len> &roll, size_t current_player=0);
     virtual std::string execute(State &state, Evaluator &evaluator);
};




CREATE_COMMAND_PARSER_DECLARATION(Autoplay);

class AutoplayCommandExecuter: public CommandExecuter{
     size_t seed;
     size_t start_player;
public: 
     AutoplayCommandExecuter(size_t seed, size_t start_player);
     virtual std::string execute(State &state, Evaluator &evaluator);
};



CREATE_COMMAND_PARSER_DECLARATION(Save);

class SaveCommandExecuter: public CommandExecuter{
     std::string filename;
public: 
     SaveCommandExecuter(const std::string &filename);
     virtual std::string execute(State &state, Evaluator &evaluator);
};


CREATE_COMMAND_PARSER_DECLARATION(Load);

class LoadCommandExecuter: public CommandExecuter{
     std::string filename;
public: 
     LoadCommandExecuter(const std::string &filename);
     virtual std::string execute(State &state, Evaluator &evaluator);
};


//Help:

CREATE_COMMAND_DECLARATION(Help);

class CommandDescriptionCommandExecuter: public CommandExecuter{
     std::string command;
public: 
     CommandDescriptionCommandExecuter(const std::string &command);
     virtual std::string execute(State &state, Evaluator &evaluator);
};

