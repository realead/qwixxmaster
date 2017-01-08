#include "Evaluator.h"

#include <algorithm>
#include <iostream>
#include <memory>

#include "State.h"
#include "RandomDice.h"
#include "BruteForceRollGenerator.h"
#include "StringUtils.h"
#include "MemoryManager.h"

namespace{
  
  size_t encode_min_max_0_based(size_t first, size_t second){
    size_t max=std::max(first, second);
    size_t min=std::min(first, second);
    return max*(max+1)/2+min;
  }

  size_t COLOR_MAX_ID=78;
  size_t calc_max_index(){
    size_t two_colors=encode_min_max_0_based(COLOR_MAX_ID, COLOR_MAX_ID);
    return (encode_min_max_0_based(two_colors, two_colors)+1)*5;
  }
  
  
  size_t calc_color_id(const ColorState &cs){
    return (cs.last-1)*cs.last/2+cs.cnt;  
  }
  
  size_t calc_2color_id(const ColorState &cs1, const ColorState &cs2){
    size_t first=calc_color_id(cs1);
    size_t second=calc_color_id(cs2);
    return encode_min_max_0_based(first, second);
  } 
  
  size_t calc_id(const State &state){
      size_t id1=calc_2color_id(state.get_color_state(cRED), state.get_color_state(cYELLOW));
      
      ColorState sGreen=state.get_color_state(cGREEN);
      sGreen.last=14-sGreen.last;
      
      ColorState sBlue=state.get_color_state(cBLUE);
      sBlue.last=14-sBlue.last;
       
      size_t id2=calc_2color_id(sGreen, sBlue);
      
      
      size_t id=encode_min_max_0_based(id1, id2);
      return id*5+state.get_missed();
  }
}

namespace {
  double STATE_NOT_EVALUATED=-300.0;//actually -16 is the worst what can happen!
  
}

Evaluator::Evaluator(size_t sampling_number_, size_t player_number_): 
   sampling_number(sampling_number_), player_number(player_number_),
   mem(player_number*(calc_max_index()+1), STATE_NOT_EVALUATED)
{}


float Evaluator::evaluate_state(const State &state){
     size_t id=calc_id(state);
     if(mem.at(id)==STATE_NOT_EVALUATED){
        if(state.ended()){
           mem.at(id)=state.score();
        }
        else{
           double value=0;
           std::unique_ptr<RollGenerator> gen;
           if(sampling_number==0)
              gen.reset(new  BruteForceRollGenerator());
           else
              gen.reset(new GlobalRollGenerator(sampling_number));
           while(gen->has_next()){
                RollPair roll_pair=gen->get_next();
                value+=roll_pair.probability*evaluate_roll(state, roll_pair.roll);
           }        
           mem.at(id)=static_cast<float>(value);
        }
     }
     
     return mem.at(id);

}

float Evaluator::evaluate_without_whites(const State &state, const DiceRoll &roll){
    // no whites!
    float best=-1e6;
    State cur=state;
    for(size_t dice=4;dice<=5;dice++){
        if(dice==5 && roll[4]==roll[5])
               break;
        for(size_t j=0;j<COLOR_CNT;j++){
            Color color=static_cast<Color>(j);
            unsigned char save_last=cur.last[j];
            unsigned char save_cnt=cur.cnt[j];
            if(cur.take(color, roll[color]+roll[dice])){
                   best=std::max(best, evaluate_state(cur));
                   cur.last[j]=save_last;
                   cur.cnt[j]=save_cnt;
            }
        }
    } 
    return best;
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
            best=std::max(best, evaluate_without_whites(cur, roll));
        }
    }
     
    return std::max(best, evaluate_without_whites(state, roll));
}



void Evaluator::evaluate_without_whites(const State &state, const DiceRoll &roll, MoveInfos &res, const std::string &prefix){
    for(size_t dice=4;dice<=5;dice++)
        for(size_t j=0;j<COLOR_CNT;j++){
            Color color=static_cast<Color>(j);
            State cur=state;
            if(cur.take(color, roll[color]+roll[dice]))
                       res.push_back(std::make_pair(evaluate_state(cur),prefix+ color2str(color)+" "+stringutils::int2str(roll[color]+roll[dice])));
        }
}

Evaluator::MoveInfos Evaluator::get_roll_evaluation(const State &state, const DiceRoll &roll){
   MoveInfos res;
   
   //always an option: take miss
   State cur=state;
   cur.add_miss();
   res.push_back(std::make_pair(evaluate_state(cur), "miss"));
   
   //take whites:
   for (size_t i=0;i<COLOR_CNT;i++){
        Color color=static_cast<Color>(i);
        cur=state;
        if(cur.take(color, roll.at(4)+roll.at(5))){
            std::string move=color2str(color)+" "+stringutils::int2str(roll.at(4)+roll.at(5));
            res.push_back(std::make_pair(evaluate_state(cur), move));
            
            //additional color dice?
            evaluate_without_whites(cur, roll, res, move+",");
        }
    }
    
    // no whites!
    evaluate_without_whites(state, roll, res, "");
    sort(res.begin(), res.end(), std::greater<MoveInfo>());
    return res;

}



Evaluator::MoveInfos Evaluator::get_roll_evaluation(const State &state, const ShortDiceRoll &roll){
   MoveInfos res;
   
   //I'm allowed not to take anything:
   res.push_back(std::make_pair(evaluate_state(state), "nothing"));
   
   //take whites:
   for (size_t i=0;i<COLOR_CNT;i++){
        Color color=static_cast<Color>(i);
        State cur=state;
        if(cur.take(color, roll.at(0)+roll.at(1))){
            std::string move=color2str(color)+" "+stringutils::int2str(roll.at(0)+roll.at(1));
            res.push_back(std::make_pair(evaluate_state(cur), move));
            
        }
    }
    
    sort(res.begin(), res.end(), std::greater<MoveInfo>());
    return res;  
}


std::pair<size_t, size_t> Evaluator::save_memory_to_file(const std::string &filename) const{
   MemoryManager::save_memory(filename, mem);
   size_t not_set=std::count(mem.begin(), mem.end(), STATE_NOT_EVALUATED);
   return std::make_pair(mem.size(), not_set);
}

std::pair<size_t, size_t> Evaluator::load_memory_from_file(const std::string &filename){
  MemoryManager::load_memory(filename, mem);
  size_t not_set=std::count(mem.begin(), mem.end(), STATE_NOT_EVALUATED);
  return std::make_pair(mem.size(), not_set);
}

