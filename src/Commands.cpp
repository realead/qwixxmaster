#include "Commands.h"

#include <sstream>


#include "StringUtils.h"
#include "State.h"
#include "QuixxException.h"

#define REGISTER_COMMAND(Command)\
    static bool registered_##Command=CommandDictionary::register_command(Command##CommandParser::command_name(), new Command##CommandParser());

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


namespace{
  void check_command_without_parameters(const CommandLine &line, const std::string &command_name){
    if(line.size()!=1)
        THROW_QUIXX("cannot parse '"<<stringutils::join(line)<<"'. The right syntax for the "<<command_name<<" command is '"<<command_name<<"'.");
    if(line[0]!=command_name) //paranoia
        THROW_QUIXX("this is not the "<<command_name<<" command");
  }
 
}

std::string ScoreCommandExecuter::execute(State &state, Evaluator &evaluator){
    return stringutils::int2str(state.score());
}

std::string ScoreCommandParser::command_name(){ 
    return "score";
}

CommandExecuterPtr ScoreCommandParser::parse(const CommandLine &line){
    check_command_without_parameters(line, command_name());
    return CommandExecuterPtr(new ScoreCommandExecuter());
}



///
std::string ExitCommandExecuter::execute(State &state, Evaluator &evaluator){
    return "";
}

bool ExitCommandExecuter::exit_program() const{
    return true;
}

std::string ExitCommandParser::command_name(){ 
    return "exit";
}

CommandExecuterPtr ExitCommandParser::parse(const CommandLine &line){   
    check_command_without_parameters(line, command_name());
    return CommandExecuterPtr(new ExitCommandExecuter());
}

///
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

std::string PrintCommandParser::command_name(){ 
    return "print";
}

CommandExecuterPtr PrintCommandParser::parse(const CommandLine &line){   
    check_command_without_parameters(line, command_name());
    return CommandExecuterPtr(new PrintCommandExecuter());
}


///
std::string TakeMissCommandExecuter::execute(State &state, Evaluator &evaluator){
    state.add_miss();
    return "";
}


std::string TakeCommandParser::command_name(){ 
    return "take";
}


namespace{
    void wrong_take_syntax(const CommandLine &command){
        THROW_QUIXX("unknown syntax: '"<<stringutils::join(command)<<"'. Known syntax either 'take miss' or 'take <color> <number>'");
    }
}


CommandExecuterPtr TakeCommandParser::parse(const CommandLine &line){   
    if(line.size()==2){
        if(line.at(1)!="miss"){
            wrong_take_syntax(line);
        }
        return CommandExecuterPtr(new TakeMissCommandExecuter());
    }
    //normal take
    if(line.size()!=3){
        wrong_take_syntax(line);
    }
    Color color;
    if(!str2color(line.at(1), color)){
        THROW_QUIXX("unknown color '"<<line.at(1)<<"'. Known colors are 'red', 'yellow', 'green', and 'blue'");
    }
    int number;
    if(!stringutils::str2int(line.at(2), number)){
       THROW_QUIXX("could not convert '"<<line.at(2)<<"' to number");
    }
    
    return CommandExecuterPtr(new TakeColorCommandExecuter(color, number));
}

TakeColorCommandExecuter::TakeColorCommandExecuter(Color color_, int number_):
   color(color_), number(number_){}
   
   
std::string TakeColorCommandExecuter::execute(State &state, Evaluator &evaluator){
    if(!state.take(color, number)){
            THROW_QUIXX("invalid move");
    }
    return "";
}


std::string SetCommandParser::command_name(){ 
    return "set";
}

CommandExecuterPtr SetCommandParser::parse(const CommandLine &line){ 
    //format of set: <taken_red> <last_red> <taken_yellow> <last_yellow> <taken_green> <last_green> <taken_blue> <last_blue> <missed>
    if(line.size()!=10)
        THROW_QUIXX("invalid syntax: '"<<stringutils::join(line)
                        <<"'. Known syntax is 'set <taken_red> <last_red>"
                          " <taken_yellow> <last_yellow> <taken_green> <last_green>"
                          " <taken_blue> <last_blue> <missed>'");       

    std::array<int,COLOR_CNT> taken;
    std::array<int,COLOR_CNT> last;
    for(size_t i=0;i<COLOR_CNT;i++){
            if(!stringutils::str2int(line.at(i*2+1), taken[i]) ||
               !stringutils::str2int(line.at(i*2+2), last[i])){
               THROW_QUIXX("cannot parse for color "<<color2str(static_cast<Color>(i)));   
            }
    }
    int missed;
    if(!stringutils::str2int(line.back(), missed)){
            THROW_QUIXX("cannot parse missed");
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
               THROW_QUIXX("invalid set for color "<<color2str(color));
        }
    }
    
    if(!new_state.set_missed(missed)){
        THROW_QUIXX("invalid missed");
    }
    state=new_state;
    
    return "";
}


//
std::string PossibleCommandParser::command_name(){ 
    return "possible";
}

CommandExecuterPtr PossibleCommandParser::parse(const CommandLine &line){   
    //normal take
    if(line.size()!=3){
       THROW_QUIXX("unknown syntax: '"<<stringutils::join(line)<<"'. Known syntax is 'take <color> <number>'");
    }
    Color color;
    if(!str2color(line.at(1), color)){
        THROW_QUIXX("unknown color '"<<line.at(1)<<"'. Known colors are 'red', 'yellow', 'green', and 'blue'");
    }
    int number;
    if(!stringutils::str2int(line.at(2), number)){
       THROW_QUIXX("could not convert '"<<line.at(2)<<"' to number");
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
std::string EndedCommandParser::command_name(){ 
    return "ended";
}

CommandExecuterPtr EndedCommandParser::parse(const CommandLine &line){   
    check_command_without_parameters(line, command_name());  
    return CommandExecuterPtr(new EndedCommandExecuter());
}
   
std::string EndedCommandExecuter::execute(State &state, Evaluator &evaluator){
    if(state.ended())
        return "Yes";
    return "No";
}

///////Restart
std::string RestartCommandParser::command_name(){ 
    return "restart";
}

CommandExecuterPtr RestartCommandParser::parse(const CommandLine &line){   
    check_command_without_parameters(line, command_name());  
    return CommandExecuterPtr(new RestartCommandExecuter());
}
   
std::string RestartCommandExecuter::execute(State &state, Evaluator &evaluator){
    state=State();
    return "";
}


//Evaluate
std::string EvaluateCommandExecuter::execute(State &state, Evaluator &evaluator){
    std::stringstream ss;
    ss<<"Expected score: "<<evaluator.evaluate_state(state);
    return ss.str();
}


std::string EvaluateCommandParser::command_name(){ 
    return "evaluate";
}

CommandExecuterPtr EvaluateCommandParser::parse(const CommandLine &line){
    check_command_without_parameters(line, command_name());
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
    Evaluator::MoveInfos infos=evaluator.get_roll_evaluation(state, roll);
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


std::string RollCommandParser::command_name(){ 
    return "roll";
}

namespace{
    template<size_t len>
    std::array<int, len> parse_dices(const CommandLine &line){
        std::array<int, len> res;
        for(size_t i=0;i<len;i++){         
            int dice;
            if(!stringutils::str2int(line.at(i+1), dice))
               THROW_QUIXX("could not convert '"<<line.at(i+1)<<"' to a number");
            if(dice<1 || dice>6)
               THROW_QUIXX("dice value must be between 1 and 6 inclusive but is '"<<dice<<"'");
            res[i]=dice;
        }
        return res;
    }
}


CommandExecuterPtr RollCommandParser::parse(const CommandLine &line){
    if(line.size()!=7 && line.size()!=3)
        THROW_QUIXX("unknown syntax: '"<<stringutils::join(line)<<"'. Known syntax is either 'roll <white_dice1> <white_dice2>' or 'roll <red_dice> <yellow_dice> <green_dice> <blue_dice> <white_dice1> <white_dice2>'");
     
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
                    THROW_QUIXX("we have an internal problem... call ghostbusters!");
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
      ss<<"move "<<move_number<<" expected score: "<<evaluator.evaluate_state(state)<<std::endl;
      std::vector<CommandExecuterPtr> commands=translate_to_executers(evaluator.get_roll_evaluation(state, roller.roll()));
      for(const CommandExecuterPtr &command : commands){
        command->execute(state, evaluator);
      }
      move_number++;
    }
    
    ss<<"Score: "<<state.score();
    return ss.str();
}

std::string AutoplayCommandParser::command_name(){ 
    return "autoplay";
}

CommandExecuterPtr AutoplayCommandParser::parse(const CommandLine &line){
    if(line.size()!=2)
       THROW_QUIXX("unknown syntax: '"<<stringutils::join(line)<<"'. Known syntax is 'autoplay <seed>'");
    int seed;   
    if (!stringutils::str2int(line.at(1), seed))
       THROW_QUIXX("could not convert '"<<line.at(1)<<"' to a number");
       
    return CommandExecuterPtr(new AutoplayCommandExecuter(seed));
}

AutoplayCommandExecuter::AutoplayCommandExecuter(size_t seed_):
    seed(seed_)
{}



//SAVE:

std::string SaveCommandParser::command_name(){ 
    return "save";
}

CommandExecuterPtr SaveCommandParser::parse(const CommandLine &line){
    if(line.size()!=2)
       THROW_QUIXX("unknown syntax: '"<<stringutils::join(line)<<"'. Known syntax is 'save <filename>'");

    return CommandExecuterPtr(new SaveCommandExecuter(line.at(1)));
}

SaveCommandExecuter::SaveCommandExecuter(const std::string &filename_):
    filename(filename_)
{}

std::string SaveCommandExecuter::execute(State &state, Evaluator &evaluator){
    evaluator.save_memory_to_file(filename);
    return "";
}


//LOAD:
std::string LoadCommandParser::command_name(){ 
    return "load";
}

CommandExecuterPtr LoadCommandParser::parse(const CommandLine &line){
    if(line.size()!=2)
       THROW_QUIXX("unknown syntax: '"<<stringutils::join(line)<<"'. Known syntax is 'load <filename>'");

    return CommandExecuterPtr(new LoadCommandExecuter(line.at(1)));
}

LoadCommandExecuter::LoadCommandExecuter(const std::string &filename_):
    filename(filename_)
{}

std::string LoadCommandExecuter::execute(State &state, Evaluator &evaluator){
    evaluator.load_memory_from_file(filename);
    return "";
}

