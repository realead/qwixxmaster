#pragma once

#include <vector>
#include <string>
#include <memory>

typedef std::vector<std::string> CommandLine;

class State;

class Command{
public:
    virtual std::string check_syntax(const CommandLine &line)=0;
    virtual std::string check_validity(const State &state, const CommandLine &line)=0;
    virtual std::string execute(State &state, const CommandLine &line)=0;
    virtual bool exit_program()=0;
    virtual ~Command(){};
};

typedef std::unique_ptr<Command> CommandPtr;

//singleton!
namespace CommandDictionary{
    bool register_command(const std::string& command_name, Command *command);
    Command &get_command(const std::string& command_name); 
};



