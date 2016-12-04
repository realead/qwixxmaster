#include "State.h"


std::string color2str(Color color){
    switch(color){
        case red: return "red";
        case yellow: return "yellow";
        case green: return "green";
        case blue: return "blue";
        default:
            return "None";
    }
}

State::State(): last{0}, cnt{0}, missed(0){};

State::ColorState State::get_color_state(Color color) const{
  return std::make_pair(cnt[color],  last[color]);
}


int State::get_missed() const{
    return missed;
}

