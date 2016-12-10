#include "Command.h"

#include <map>

#include "QuixxException.h"


namespace{
    
    typedef std::map<std::string, CommandPtr> CommandMap;
    CommandMap &get_command_map(){
        static CommandMap map;
        return map;
    }
}

bool CommandDictionary::register_command(const std::string& command_name, Command *command){
    if(command==NULL)
        THROW_QUIXX("Trying to register NULL as command - this is not allowed");
        
    CommandPtr c(command);
    CommandMap &map=get_command_map();
    if(map.count(command_name)>0)   
        THROW_QUIXX("Trying to register command "<<command_name<<" twice");
    map[command_name]=std::move(c);
    return true; 
}

Command &CommandDictionary::get_command(const std::string& command_name){
    CommandMap::const_iterator it=get_command_map().find(command_name);
    if(it==get_command_map().end())
        THROW_QUIXX("Unknown command "<< command_name);
    return *(it->second);
}


