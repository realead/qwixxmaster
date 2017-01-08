#include "Command.h"

#include <map>
#include <algorithm>

#include "QwixxException.h"
#include "StringUtils.h"


namespace{
    
    typedef std::map<std::string, CommandParserPtr> CommandMap;
    CommandMap &get_command_map(){
        static CommandMap map;
        return map;
    }
}

bool CommandDictionary::register_command(CommandParser *command){
    if(command==NULL)
        THROW_QWIXX("Trying to register NULL as command - this is not allowed");
        
    CommandParserPtr c(command);
    const std::string command_name=c->command_name(); 
    CommandMap &map=get_command_map();
    if(map.count(command_name)>0)   
        THROW_QWIXX("Trying to register command "<<command_name<<" twice");
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
        THROW_QWIXX("unknown command: "<< command_name);
    return *(it->second);
}

CommandExecuterPtr CommandParser::parse(const CommandLine &line){
    //paranoia:
    if(line.empty() || line.front()!=command_name())
       THROW_QWIXX("this is not the command: "<< command_name());
    
    std::vector<size_t> arg_cnts=possible_argument_cnt();
    if(std::count(arg_cnts.begin(), arg_cnts.end(), line.size()-1)==0){
       wrong_syntax(line);
    }
    
    return parse_inner(line);
}

 void CommandParser::wrong_syntax(const CommandLine &line) const{
    THROW_QWIXX("unknown syntax: '"<<stringutils::join(line)<<"'. Known syntax for the '"<< command_name() << "' command is "<< usage()<<".");
 }

