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
 


State::State(): last{0}, cnt{0}, missed(0){};

ColorState State::get_color_state(Color color) const{
  return ColorState( last[color], cnt[color]);
}

int State::get_missed() const{
    return missed;
}


bool State::take(Color color, int number){
   if(number<0 || number>12)
        return false;
   unsigned char candidate=static_cast<unsigned char>(number);
   if(candidate<=last[color])
        return false;
        
   //check ok, change
   last[color]=candidate;
   cnt[color]++;
   if(candidate==12 && cnt[color]>=5)
    cnt[color]++;//closed!
   return true;
}

void State::add_miss(){
  missed++;
}

