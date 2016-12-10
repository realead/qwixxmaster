#include "Commands.h"

#include "StringUtils.h"
#include "State.h"
#include "QuixxException.h"

#define REGISTER_COMMAND(Type)\
    static bool registered_##Type=CommandDictionary::register_command(Type::command_name(), new Type());

REGISTER_COMMAND(ScoreCommandParser);
REGISTER_COMMAND(ExitCommandParser);


namespace{
  void check_command_without_parameters(const CommandLine &line, const std::string &command_name){
    if(line.size()!=1)
        THROW_QUIXX("can not parse '"<<stringutils::join(line)<<"'. The right syntax for the "<<command_name<<" command is '"<<command_name<<"'.");
    if(line[0]!=command_name) //paranoia
        THROW_QUIXX("this is not the "<<command_name<<" command");
  }
 
}

std::string ScoreCommandExecuter::execute(State &state){
    return stringutils::int2str(state.score());
}

bool ScoreCommandExecuter::exit_program(){
    return false;
}

std::string ScoreCommandParser::command_name(){ 
    return "score";
}

CommandExecuterPtr ScoreCommandParser::parse(const CommandLine &line){
    check_command_without_parameters(line, command_name());
    return CommandExecuterPtr(new ScoreCommandExecuter());
}



///
std::string ExitCommandExecuter::execute(State &state){
    return "";
}

bool ExitCommandExecuter::exit_program(){
    return true;
}

std::string ExitCommandParser::command_name(){ 
    return "exit";
}

CommandExecuterPtr ExitCommandParser::parse(const CommandLine &line){   
    check_command_without_parameters(line, command_name());
    return CommandExecuterPtr(new ExitCommandExecuter());
}


