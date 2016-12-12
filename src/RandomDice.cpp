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


