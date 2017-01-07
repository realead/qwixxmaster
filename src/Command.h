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
    virtual bool exit_program() const {return false;};
    virtual ~CommandExecuter(){};
};

typedef std::unique_ptr<CommandExecuter> CommandExecuterPtr;


class CommandParser{
    virtual CommandExecuterPtr parse_inner(const CommandLine &line)=0;
    virtual std::vector<size_t> possible_argument_cnt() const = 0;
protected:
    void wrong_syntax(const CommandLine &line) const;
public:
    CommandExecuterPtr parse(const CommandLine &line);
    virtual std::string command_name() const=0;
    virtual std::string usage() const=0;
    virtual std::string description() const=0;
    virtual ~CommandParser(){};
};

typedef std::unique_ptr<CommandParser> CommandParserPtr;

//singleton!
namespace CommandDictionary{
    bool register_command(CommandParser *command);
    CommandExecuterPtr get_command_executer(const CommandLine &line);
    std::vector<std::string> get_registered_command_names();
    CommandParser &get_command_parser(const std::string &command_name);
};



