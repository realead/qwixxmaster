#include "Evaluator.h"

#include <algorithm>
#include <iostream>

#include "State.h"
#include "RandomDice.h"

namespace{

  size_t COLOR_MAX_ID=79;
  size_t calc_max_index(){
    return 79*79*79*79*5;
  }
  
  size_t calc_id(const State &state){
      size_t id=0;
      for(size_t i=0;i<COLOR_CNT;i++){
         ColorState cs=state.get_color_state(static_cast<Color>(i));
         if(i>=2)//green/blue are 12..2
            cs.last=14-cs.last;
         size_t color_id=(cs.last-1)*cs.last/2+cs.cnt;
         id=id*COLOR_MAX_ID+color_id;
      }
      return id*5+state.get_missed();
  }
}


Evaluator::Evaluator(size_t sampling_number_): 
   sampling_number(sampling_number_),
   mem(calc_max_index()+1, -1.0)
{}


float Evaluator::evaluate_state(const State &state){
     size_t id=calc_id(state);
     if(mem.at(id)<0.0){
        if(state.ended()){
           mem.at(id)=state.score();
        }
        else{
           float value=0;
           for(size_t i=0;i<sampling_number;i++)
              value+=evaluate_roll(state, RandomDice::random_roll());
                    
           mem.at(id)=value/sampling_number;
        }
     }
     
     return mem.at(id);

}


float Evaluator::evaluate_roll(const State &state, const DiceRoll &roll){

   //always an option: take miss
   State cur=state;
   cur.add_miss();
   float best=evaluate_state(cur);
   
   //take whites:
   for (size_t i=0;i<COLOR_CNT;i++){
        Color color=static_cast<Color>(i);
        cur=state;
        if(cur.take(color, roll[4]+roll[5])){
            best=std::max(best, evaluate_state(cur));
            
            //maybe additional color dices can be taken?
            for(size_t dice=4;dice<=5;dice++)
                for(size_t j=0;j<COLOR_CNT;j++){
                    Color color2=static_cast<Color>(i);
                    State cur2=cur;
                    if(cur2.take(color2, roll[color]+roll[dice]))
                       best=std::max(best, evaluate_state(cur2));
                }
        }
    }
    
    // no whites!
    for(size_t dice=4;dice<=5;dice++)
        for(size_t j=0;j<COLOR_CNT;j++){
            Color color=static_cast<Color>(j);
                 cur=state;
                 if(cur.take(color, roll[color]+roll[dice]))
                       best=std::max(best, evaluate_state(cur));
        }  
    
    return best;
}

