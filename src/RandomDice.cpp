#include "RandomDice.h"

#include <random>
#include <algorithm>


DiceRoll RandomDice::random_roll(){
   static std::mt19937 gen(42);
   DiceRoll res;
   for(size_t i=0;i<6;i++)
     res.at(i)=1+gen()%6;
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
 
 
 
GlobalRollGenerator::GlobalRollGenerator(size_t sampling_number):
   left(sampling_number), prob(1.0/sampling_number){};

bool GlobalRollGenerator::has_next() const{
    return left>0;
}

RollPair GlobalRollGenerator::get_next(){
    left--;
    return RollPair(prob, RandomDice::random_roll());
}




