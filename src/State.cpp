#include "State.h"


std::string color2str(Color color){
    switch(color){
        case cRED: return "red";
        case cYELLOW: return "yellow";
        case cGREEN: return "green";
        case cBLUE: return "blue";
        //default:
        //    return "None";
    }
}


bool str2color(const std::string &str, Color &color){
    for(size_t i=0;i<=cBLUE;i++){
       color=static_cast<Color>(i);
       if(str==color2str(color))
            return true;    
    }
    return false;
}

ColorState::ColorState(unsigned char last_, unsigned char cnt_):
   last(last_), cnt(cnt_)
{}   
 


State::State(): last{1,1,13,13}, cnt{0}, missed(0){};

ColorState State::get_color_state(Color color) const{
  return ColorState( last[color], cnt[color]);
}

int State::get_missed() const{
    return missed;
}


bool State::take(Color color, int number){
   if (!take_possible(color, number))
      return false;
        
   //check ok, change
   unsigned char candidate=static_cast<unsigned char>(number);
   last[color]=candidate;
   cnt[color]++;
   if(candidate==12 && cnt[color]>=5)
    cnt[color]++;//closed!
   return true;
}


bool State::take_possible(Color color, int number) const{
   if(number<0 || number>12)
        return false;
   unsigned char candidate=static_cast<unsigned char>(number);
   if( (color==cRED || color==cYELLOW) && candidate<=last[color])
        return false;
   if( (color==cGREEN || color==cBLUE) && candidate>=last[color])
        return false;
   return true;
}


bool State::set(Color color, int last_, int taken_){
    //TODO: check if valid
    last[color]=static_cast<unsigned char>(last_);
    cnt[color]=static_cast<unsigned char>(taken_);
    
    return true;
}


void State::add_miss(){
    missed++;
}

bool State::set_missed(int missed_){
    //TODO: check if valid
    missed=static_cast<unsigned char>(missed_);
    return true;
}

const int scores[13]={0, 1, 3, 6, 10, 15, 21, 28, 36, 45, 55, 66, 78};
int State::score() const{
      int res=0;
      for(size_t i=0;i<COLOR_CNT;i++)
            res+=scores[cnt[i]];
      return res-5*missed;
}



