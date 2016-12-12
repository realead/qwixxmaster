#pragma once

#include <vector>
#include <string>
#include <memory>

typedef std::vector<std::string> CommandLine;

class State;
class Evaluator;

class CommandExecuter{
public:
    virtual std::string execute(State &state, Evaluator &evaluator)=0;
    virtual bool exit_program()=0;
    virtual ~CommandExecuter(){};
};

typedef std::unique_ptr<CommandExecuter> CommandExecuterPtr;


class CommandParser{
public:
    virtual CommandExecuterPtr parse(const CommandLine &line)=0;
    virtual ~CommandParser(){};
};

typedef std::unique_ptr<CommandParser> CommandParserPtr;

//singleton!
namespace CommandDictionary{
    bool register_command(const std::string& command_name, CommandParser *command);
    CommandExecuterPtr get_command_executer(const CommandLine &line);
};



