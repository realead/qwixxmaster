#include "Commands.h"

#include <sstream>


#include "StringUtils.h"
#include "State.h"
#include "QwixxException.h"

#define REGISTER_COMMAND(Command)\
    static bool registered_##Command=CommandDictionary::register_command(new Command##CommandParser());

REGISTER_COMMAND(Score);
REGISTER_COMMAND(Exit);
REGISTER_COMMAND(Print);
REGISTER_COMMAND(Take);
REGISTER_COMMAND(Set);
REGISTER_COMMAND(Possible);
REGISTER_COMMAND(Ended);
REGISTER_COMMAND(Restart);


REGISTER_COMMAND(Evaluate);
REGISTER_COMMAND(Roll);
REGISTER_COMMAND(Autoplay);


REGISTER_COMMAND(Save);
REGISTER_COMMAND(Load);

REGISTER_COMMAND(Help);


std::string ScoreCommandExecuter::execute(State &state, Evaluator &evaluator){
    return stringutils::int2str(state.score());
}

std::string ScoreCommandParser::command_name() const{ 
    return "score";
}

std::string ScoreCommandParser::usage() const{
   return "'score'";
}

std::string ScoreCommandParser::description() const{
    return "prints the score of the current state";
}

std::vector<size_t> ScoreCommandParser::possible_argument_cnt() const{ 
    return {0};
}

CommandExecuterPtr ScoreCommandParser::parse_inner(const CommandLine &line){
    return CommandExecuterPtr(new ScoreCommandExecuter());
}



///Exit
std::string ExitCommandExecuter::execute(State &state, Evaluator &evaluator){
    return "";
}

std::string ExitCommandParser::command_name() const{ 
    return "exit";
}

std::string ExitCommandParser::usage() const{
   return "'exit'";
}

std::string ExitCommandParser::description() const{
    return "ends the program, the same as Ctrl+D";
}

std::vector<size_t> ExitCommandParser::possible_argument_cnt() const{ 
    return {0};
}

bool ExitCommandExecuter::exit_program() const{
    return true;
}


CommandExecuterPtr ExitCommandParser::parse_inner(const CommandLine &line){  
    return CommandExecuterPtr(new ExitCommandExecuter());
}

///Print
std::string PrintCommandExecuter::execute(State &state, Evaluator &evaluator){
    std::stringstream out;
    for(size_t i=0;i<COLOR_CNT;i++){
            Color color=static_cast<Color>(i);
            ColorState cs=state.get_color_state(color);  
            out<<color2str(color)<<":\t"<<cs.cnt<<" taken, last=";
            if(cs.last>=2 && cs.last<=12)
               out<<cs.last<<std::endl;
            else
               out<<"none"<<std::endl;
        }
    out<<"missed: "<<state.get_missed();
    return out.str();
}

std::string PrintCommandParser::command_name() const{ 
    return "print";
}

std::string PrintCommandParser::usage() const{
   return "'print'";
}

std::string PrintCommandParser::description() const{
    return "prints the current state";
}


std::vector<size_t> PrintCommandParser::possible_argument_cnt() const{ 
    return {0};
}

CommandExecuterPtr PrintCommandParser::parse_inner(const CommandLine &line){   
    return CommandExecuterPtr(new PrintCommandExecuter());
}


/// Take
std::string TakeMissCommandExecuter::execute(State &state, Evaluator &evaluator){
    state.add_miss();
    return "";
}

std::string TakeCommandParser::command_name() const{ 
    return "take";
}

std::string TakeCommandParser::usage() const{
   return "either 'take miss' or 'take <color> <number>'";
}

std::string TakeCommandParser::description() const{
    return "takes the given combination of color/number or a miss";
}

std::vector<size_t> TakeCommandParser::possible_argument_cnt() const{ 
    return {1,2};
}

CommandExecuterPtr TakeCommandParser::parse_inner(const CommandLine &line){   
    if(line.size()==2){
        if(line.at(1)!="miss"){
            wrong_syntax(line);
        }
        return CommandExecuterPtr(new TakeMissCommandExecuter());
    }
    //normal take
    Color color;
    if(!str2color(line.at(1), color)){
        THROW_QWIXX("unknown color '"<<line.at(1)<<"'. Known colors are 'red', 'yellow', 'green', and 'blue'");
    }
    int number;
    if(!stringutils::str2int(line.at(2), number)){
       THROW_QWIXX("could not convert '"<<line.at(2)<<"' to number");
    }
    
    return CommandExecuterPtr(new TakeColorCommandExecuter(color, number));
}

TakeColorCommandExecuter::TakeColorCommandExecuter(Color color_, int number_):
   color(color_), number(number_){}
   
   
std::string TakeColorCommandExecuter::execute(State &state, Evaluator &evaluator){
    if(!state.take(color, number)){
            THROW_QWIXX("invalid move");
    }
    return "";
}




//SET:
std::string SetCommandParser::command_name() const{ 
    return "set";
}

std::string SetCommandParser::usage() const{
   return "'set <taken_red> <last_red>"
          " <taken_yellow> <last_yellow> <taken_green> <last_green>"
          " <taken_blue> <last_blue> <missed>'";
}

std::string SetCommandParser::description() const{
    return "sets the current state to a given state";
}

std::vector<size_t> SetCommandParser::possible_argument_cnt() const{ 
    return {9};
}


CommandExecuterPtr SetCommandParser::parse_inner(const CommandLine &line){      
    std::array<int,COLOR_CNT> taken;
    std::array<int,COLOR_CNT> last;
    for(size_t i=0;i<COLOR_CNT;i++){
            if(!stringutils::str2int(line.at(i*2+1), taken[i]) ||
               !stringutils::str2int(line.at(i*2+2), last[i])){
               THROW_QWIXX("cannot parse for color "<<color2str(static_cast<Color>(i)));   
            }
    }
    int missed;
    if(!stringutils::str2int(line.back(), missed)){
            THROW_QWIXX("cannot parse missed");
    }  
    return CommandExecuterPtr(new SetCommandExecuter(last, taken, missed)); 
}


SetCommandExecuter::SetCommandExecuter(const std::array<int,COLOR_CNT> &last_, const std::array<int,COLOR_CNT> &taken_, int missed_):
   last(last_), taken(taken_), missed(missed_){}
   
   
std::string SetCommandExecuter::execute(State &state, Evaluator &evaluator){
    State new_state(state);
    for(size_t i=0;i<COLOR_CNT;i++){
        Color color=static_cast<Color>(i); 
        if(!new_state.set(color, last[i], taken[i])){
               THROW_QWIXX("invalid set for color "<<color2str(color));
        }
    }
    
    if(!new_state.set_missed(missed)){
        THROW_QWIXX("invalid missed");
    }
    state=new_state;
    
    return "";
}


//Possible
std::string PossibleCommandParser::command_name() const{ 
    return "possible";
}


std::string PossibleCommandParser::usage() const{
   return "'possible <color> <number>'";
}

std::string PossibleCommandParser::description() const{
    return "prints 'Yes' if the taking of a color/number combination is possible; prints 'No' otherwise";
}

std::vector<size_t> PossibleCommandParser::possible_argument_cnt() const{ 
    return {2};
}


CommandExecuterPtr PossibleCommandParser::parse_inner(const CommandLine &line){   
    Color color;
    if(!str2color(line.at(1), color)){
        THROW_QWIXX("unknown color '"<<line.at(1)<<"'. Known colors are 'red', 'yellow', 'green', and 'blue'");
    }
    int number;
    if(!stringutils::str2int(line.at(2), number)){
       THROW_QWIXX("could not convert '"<<line.at(2)<<"' to number");
    }
    
    return CommandExecuterPtr(new PossibleCommandExecuter(color, number));
}

PossibleCommandExecuter::PossibleCommandExecuter(Color color_, int number_):
   color(color_), number(number_){}
   
   
std::string PossibleCommandExecuter::execute(State &state, Evaluator &evaluator){
    if(state.take_possible(color, number))
        return "Yes";
    return "No";
}



///////Ended
std::string EndedCommandParser::command_name() const{ 
    return "ended";
}


std::string EndedCommandParser::usage() const{
   return "'ended'";
}

std::string EndedCommandParser::description() const{
    return "prints 'Yes' if the current state means the end of the game; prints 'No' otherwise";
}

std::vector<size_t> EndedCommandParser::possible_argument_cnt() const{ 
    return {0};
}


CommandExecuterPtr EndedCommandParser::parse_inner(const CommandLine &line){   
    return CommandExecuterPtr(new EndedCommandExecuter());
}
   
std::string EndedCommandExecuter::execute(State &state, Evaluator &evaluator){
    if(state.ended())
        return "Yes";
    return "No";
}


///////Restart
std::string RestartCommandParser::command_name() const{ 
    return "restart";
}


std::string RestartCommandParser::usage() const{
   return "'restart'";
}

std::string RestartCommandParser::description() const{
    return "restarts the game - the current state is the initial state";
}

std::vector<size_t> RestartCommandParser::possible_argument_cnt() const{ 
    return {0};
}


CommandExecuterPtr RestartCommandParser::parse_inner(const CommandLine &line){  
    return CommandExecuterPtr(new RestartCommandExecuter());
}
   
std::string RestartCommandExecuter::execute(State &state, Evaluator &evaluator){
    state=State();
    return "";
}


//Evaluate
std::string EvaluateCommandExecuter::execute(State &state, Evaluator &evaluator){
    std::stringstream ss;
    ss<<"Expected score: "<<evaluator.evaluate_state(state, 0);
    return ss.str();
}


std::string EvaluateCommandParser::command_name() const{ 
    return "evaluate";
}


std::string EvaluateCommandParser::usage() const{
   return "'evaluate'";
}

std::string EvaluateCommandParser::description() const{
    return "evaluate the states and prints expected score for a perfect strategy";
}
std::vector<size_t> EvaluateCommandParser::possible_argument_cnt() const{ 
    return {0};
}


CommandExecuterPtr EvaluateCommandParser::parse_inner(const CommandLine &line){
    return CommandExecuterPtr(new EvaluateCommandExecuter());
}


//
template <size_t  len>
RollCommandExecuter<len>::RollCommandExecuter(const std::array<int, len> &roll_):
   roll(roll_)
{}


template <size_t  len>
std::string RollCommandExecuter<len>::execute(State &state, Evaluator &evaluator){
    std::stringstream ss;
    Evaluator::MoveInfos infos=evaluator.get_roll_evaluation(state, roll, 0);
    size_t cnt=0;
    for(const Evaluator::MoveInfo &info:infos){
           if(cnt!=0)
                ss<<std::endl;
           ss<<info.second<<": "<<info.first;
           cnt++;
    }
    return ss.str();
}


template class RollCommandExecuter<6>;


std::string RollCommandParser::command_name() const{ 
    return "roll";
}


std::string RollCommandParser::usage() const{
   return "either 'roll <white_dice1> <white_dice2>'"
          " or "
          "'roll <red_dice> <yellow_dice> <green_dice> <blue_dice> <white_dice1> <white_dice2>'";
}

std::string RollCommandParser::description() const{
    return "evaluates a dice roll (of 6 or 2 dices) and prints the expected scores for all possible decisions";
}

std::vector<size_t> RollCommandParser::possible_argument_cnt() const{ 
    return {2,6};
}


namespace{
    template<size_t len>
    std::array<int, len> parse_dices(const CommandLine &line){
        std::array<int, len> res;
        for(size_t i=0;i<len;i++){         
            int dice;
            if(!stringutils::str2int(line.at(i+1), dice))
               THROW_QWIXX("could not convert '"<<line.at(i+1)<<"' to a number");
            if(dice<1 || dice>6)
               THROW_QWIXX("dice value must be between 1 and 6 inclusive but is '"<<dice<<"'");
            res[i]=dice;
        }
        return res;
    }
}


CommandExecuterPtr RollCommandParser::parse_inner(const CommandLine &line){    
    if(line.size()==7){
        return CommandExecuterPtr(new RollCommandExecuter<6>(parse_dices<6>(line)));
    }
    //line.size()==3
    return CommandExecuterPtr(new RollCommandExecuter<2>(parse_dices<2>(line)));
}





//Autoplay
namespace{
    std::vector<CommandExecuterPtr> translate_to_executers(const Evaluator::MoveInfos &infos){
        std::vector<CommandExecuterPtr> res;
        if(infos.empty())
            return res;
            
        const Evaluator::MoveInfo &info=infos.front();
        if(info.second=="miss"){
            res.push_back(CommandExecuterPtr(new TakeMissCommandExecuter()));
            return res;
        }
        std::vector<std::string> splitted=stringutils::split(info.second, ',');
        for(const std::string &s : splitted){
            std::vector<std::string> spLine=stringutils::split(s, ' ');
            Color color; int number;
            if(!str2color(spLine.at(0), color) || !stringutils::str2int(spLine.at(1), number)){
                    THROW_QWIXX("we have an internal problem... call ghostbusters!");
            }
            res.push_back(CommandExecuterPtr(new TakeColorCommandExecuter(color, number)));
        } 
        return res;
    }
}
std::string AutoplayCommandExecuter::execute(State &state, Evaluator &evaluator){
    std::stringstream ss;
    DiceRoller roller(seed);
    size_t move_number=0;
    while(!state.ended()){
      ss<<"move "<<move_number<<" expected score: "<<evaluator.evaluate_state(state, 0)<<std::endl;
      std::vector<CommandExecuterPtr> commands=translate_to_executers(evaluator.get_roll_evaluation(state, roller.roll(), 0));
      for(const CommandExecuterPtr &command : commands){
        command->execute(state, evaluator);
      }
      move_number++;
    }
    
    ss<<"Score: "<<state.score();
    return ss.str();
}

std::string AutoplayCommandParser::command_name() const{ 
    return "autoplay";
}

std::string AutoplayCommandParser::usage() const{
   return "'autoplay <seed>'";
}

std::string AutoplayCommandParser::description() const{
    return "plays a game, dice rolls are randomly chosen and determined by given seed";
}

std::vector<size_t> AutoplayCommandParser::possible_argument_cnt() const{ 
    return {1};
}


CommandExecuterPtr AutoplayCommandParser::parse_inner(const CommandLine &line){
    if(line.size()!=2)
       THROW_QWIXX("unknown syntax: '"<<stringutils::join(line)<<"'. Known syntax is 'autoplay <seed>'");
    int seed;   
    if (!stringutils::str2int(line.at(1), seed))
       THROW_QWIXX("could not convert '"<<line.at(1)<<"' to a number");
       
    return CommandExecuterPtr(new AutoplayCommandExecuter(seed));
}

AutoplayCommandExecuter::AutoplayCommandExecuter(size_t seed_):
    seed(seed_)
{}



//SAVE:

namespace{
    std::string report2str(const std::pair<size_t, size_t> &report){         
        std::stringstream ss;
        double percent=report.second*100.0/report.first;
        ss<<report.second<<" out of "<<report.first<<" ("<<percent<<") not set";
        return ss.str();
    }
}

std::string SaveCommandParser::command_name() const{ 
    return "save";
}


std::string SaveCommandParser::usage() const{
   return "'save <filename>'";
}

std::string SaveCommandParser::description() const{
    return "saves the evaluations of the states to a given file";
}

std::vector<size_t> SaveCommandParser::possible_argument_cnt() const{ 
    return {1};
}


CommandExecuterPtr SaveCommandParser::parse_inner(const CommandLine &line){
    if(line.size()!=2)
       THROW_QWIXX("unknown syntax: '"<<stringutils::join(line)<<"'. Known syntax is 'save <filename>'");

    return CommandExecuterPtr(new SaveCommandExecuter(line.at(1)));
}

SaveCommandExecuter::SaveCommandExecuter(const std::string &filename_):
    filename(filename_)
{}

std::string SaveCommandExecuter::execute(State &state, Evaluator &evaluator){
    auto report=evaluator.save_memory_to_file(filename);
    return report2str(report);
}


//LOAD:
std::string LoadCommandParser::command_name() const{ 
    return "load";
}


std::string LoadCommandParser::usage() const{
   return "'load <filename>'";
}

std::string LoadCommandParser::description() const{
    return "loads the evaluations of the states from a given file";
}

std::vector<size_t> LoadCommandParser::possible_argument_cnt() const{ 
    return {1};
}

CommandExecuterPtr LoadCommandParser::parse_inner(const CommandLine &line){
    if(line.size()!=2)
       THROW_QWIXX("unknown syntax: '"<<stringutils::join(line)<<"'. Known syntax is 'load <filename>'");

    return CommandExecuterPtr(new LoadCommandExecuter(line.at(1)));
}

LoadCommandExecuter::LoadCommandExecuter(const std::string &filename_):
    filename(filename_)
{}

std::string LoadCommandExecuter::execute(State &state, Evaluator &evaluator){
    std::pair<size_t, size_t> report=evaluator.load_memory_from_file(filename);
    return report2str(report);
}


//Help:
std::string HelpCommandParser::command_name() const{ 
    return "help";
}


std::string HelpCommandParser::usage() const{
   return "either 'help' or 'help <command>'";
}

std::string HelpCommandParser::description() const{
    return "prints a list of all commands or, given a command name, description of a command";
}

std::vector<size_t> HelpCommandParser::possible_argument_cnt() const{ 
    return {0,1};
}

CommandExecuterPtr HelpCommandParser::parse_inner(const CommandLine &line){
    if(line.size()!=2 && line.size()!=1)
       THROW_QWIXX("unknown syntax: '"<<stringutils::join(line)<<"'. Known syntax is either 'help' or 'help <command>'");
    if(line.size()==1)
       return CommandExecuterPtr(new HelpCommandExecuter());
    return CommandExecuterPtr(new CommandDescriptionCommandExecuter(line.at(1)));
}


std::string HelpCommandExecuter::execute(State &state, Evaluator &evaluator){
    std::vector<std::string> command_names=CommandDictionary::get_registered_command_names();
    std::stringstream ss;
    ss<<"all known commands:"<<std::endl;
    for(const std::string c_name : command_names){
       ss<<"\t"<<c_name<<std::endl;
    }
    ss<<"type 'help <command name>' to get more information about a command";
    return ss.str();
}


CommandDescriptionCommandExecuter::CommandDescriptionCommandExecuter(const std::string &command_):
  command(command_)
{}


std::string CommandDescriptionCommandExecuter::execute(State &state, Evaluator &evaluator){
    const CommandParser &cp=CommandDictionary::get_command_parser(command);
    std::stringstream ss;
    ss<<command<<":"<<std::endl;
    ss<<"usage:\t"<<cp.usage()<<std::endl;
    ss<<"description:\t"<<cp.description();
    return ss.str();
}

