#include "Command.h"

#include <map>

#include "QuixxException.h"


namespace{
    
    typedef std::map<std::string, CommandParserPtr> CommandMap;
    CommandMap &get_command_map(){
        static CommandMap map;
        return map;
    }
}

bool CommandDictionary::register_command(CommandParser *command){
    if(command==NULL)
        THROW_QUIXX("Trying to register NULL as command - this is not allowed");
        
    CommandParserPtr c(command);
    const std::string command_name=c->command_name(); 
    CommandMap &map=get_command_map();
    if(map.count(command_name)>0)   
        THROW_QUIXX("Trying to register command "<<command_name<<" twice");
    map[command_name]=std::move(c);
    return true; 
}

CommandExecuterPtr CommandDictionary::get_command_executer(const CommandLine &line){  
    const std::string &command_name=line.at(0); 
    return CommandDictionary::get_command_parser(command_name).parse(line);
}


std::vector<std::string> CommandDictionary::get_registered_command_names(){
   std::vector<std::string> res;
   CommandMap::const_iterator it=get_command_map().begin();
   CommandMap::const_iterator end=get_command_map().end();
   for(;it!=end;++it)
     res.push_back(it->first);
   return res;
}

CommandParser &CommandDictionary::get_command_parser(const std::string &command_name){
   CommandMap::const_iterator it=get_command_map().find(command_name);
    if(it==get_command_map().end())
        THROW_QUIXX("unknown command: "<< command_name);
    return *(it->second);
}

CommandExecuterPtr CommandParser::parse(const CommandLine &line){
    //paranoia:
    if(line.empty() || line.front()!=command_name())
       THROW_QUIXX("this is not the command: "<< command_name());
    
    //TODO: check the number of arguments
    
    return parse_inner(line);
}

