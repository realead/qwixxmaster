#include "BruteForceRollGenerator.h"

 bool BruteForceRollGenerator::has_next() const{
    return roll.at(5)<=6;
 }
 
  BruteForceRollGenerator::BruteForceRollGenerator():
     roll{1,1,1,1,1,1}{}
     
 
 namespace{
   const double SIMPLE_PROB=1.0/(36.0*36.0*36.0);
   const double DOUBLE_PROB=SIMPLE_PROB*2.0;
 }
 
 
 
 RollPair BruteForceRollGenerator::get_next(){
   RollPair result(roll[4]==roll[5]? SIMPLE_PROB : DOUBLE_PROB, roll);
   
   
   //calc next:
   for(size_t i=0;i<6;i++){
        if(i!=5 && (roll.at(i)==6 || (i==4 && roll.at(4)==roll.at(5))))
           roll.at(i)=1;
        else{
           roll.at(i)++;
           break;
        }
   }
   
   
   return result;
 
 }
 
 
 
