#include "Commands.h"

#include "StringUtils.h"
#include "State.h"

#define REGISTER_COMMAND(Type)\
    static bool registered_##Type=CommandDictionary::register_command(Type::command_name(), new Type());

REGISTER_COMMAND(ScoreCommand);

std::string ScoreCommand::check_syntax(const CommandLine &line){
    if(line.size()!=1)
        return "can not parse '"+stringutils::join(line)+"'. The right syntax for the score command is 'score'.";
    if(line[0]!=command_name()) //paranoia
        return "this is not the score command";
    return "";
}

std::string ScoreCommand::check_validity(const State &state, const CommandLine &line){
    return ""; //always ok
}

std::string ScoreCommand::execute(State &state, const CommandLine &line){
    return stringutils::int2str(state.score());
}

bool ScoreCommand::exit_program(){
    return false;
}

std::string ScoreCommand::command_name(){ 
    return "score";
}



