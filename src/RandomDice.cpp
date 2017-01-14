#include "RandomDice.h"

#include <random>
#include <algorithm>

namespace{
    std::mt19937 &get_global_random(){
        static std::mt19937 gen(42);
        return gen;
    }
}
DiceRoll RandomDice::random_roll(){
   std::mt19937 &gen=get_global_random();
   DiceRoll res;
   std::uniform_int_distribution<int> uni(1,6);
   for(size_t i=0;i<6;i++)
     res.at(i)=uni(gen);
   return res;
}


ShortDiceRoll RandomDice::random_short_roll(){
   std::mt19937 &gen=get_global_random();
   ShortDiceRoll res;
   std::uniform_int_distribution<int> uni(1,6);
   for(size_t i=0;i<res.size();i++)
     res.at(i)=uni(gen);
   return res;
}

 DiceRoller::DiceRoller(size_t seed):
    gen(seed), distr(1,6)
 {
 
 }
 
 DiceRoll  DiceRoller::roll(){
     DiceRoll res;
       for(size_t i=0;i<6;i++)
         res.at(i)=distr(gen);
       return res;
 }
 
 ShortDiceRoll DiceRoller::short_roll(){
      ShortDiceRoll res;
      res.at(0)=distr(gen);
      res.at(1)=distr(gen);
      return res;
 }
 
 
//full generator
GlobalRollGenerator::GlobalRollGenerator(size_t sampling_number):
   left(sampling_number), prob(1.0/sampling_number){};

bool GlobalRollGenerator::has_next() const{
    return left>0;
}

RollPair GlobalRollGenerator::get_next(){
    left--;
    return RollPair(prob, RandomDice::random_roll());
}


//Short generator 
GlobalShortRollGenerator::GlobalShortRollGenerator(size_t sampling_number):
   left(sampling_number), prob(1.0/sampling_number){};

bool GlobalShortRollGenerator::has_next() const{
    return left>0;
}

ShortRollPair GlobalShortRollGenerator::get_next(){
    left--;
    return ShortRollPair(prob, RandomDice::random_short_roll());
}


