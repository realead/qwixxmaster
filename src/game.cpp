#include "game.h"

#include <iostream>



namespace{
    void print_state(const State &state, std::ostream &out){
        for(size_t i=0;i<COLOR_CNT;i++){
            Color color=static_cast<Color>(i);
            State::ColorState cs=state.get_color_state(color);
            out<<color2str(color)<<":\t"<<cs.second<<" taken, last="<<cs.first<<std::endl;
        }
        out<<"missed: "<<state.get_missed()<<std::endl;
    }

}


bool game::execute_command(const std::vector<std::string> &command, std::ostream &out){
    if(command.empty())
        return true;
    if(command.at(0)=="exit")
        return false;
    if(command.at(0)=="print"){
        print_state(state, out);
        return true;
    }
        
    
    //unknown!
    out<<"unknown command: "<<command.at(0)<<std::endl;
    return true;
}


