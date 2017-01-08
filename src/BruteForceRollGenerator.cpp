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
 
 
 
 
 
 //SHORT GENERATOR
bool BruteForceShortRollGenerator::has_next() const{
    return roll.at(1)<=6;
 }
 
BruteForceShortRollGenerator::BruteForceShortRollGenerator():
     roll{1,1}
{}
     
 
 namespace{
   const double SHORT_SIMPLE_PROB=1.0/(36.0);
   const double SHORT_DOUBLE_PROB=SHORT_SIMPLE_PROB*2.0;
 }
 
 
 
 ShortRollPair BruteForceShortRollGenerator::get_next(){
   ShortRollPair result(roll[0]==roll[1]? SHORT_SIMPLE_PROB : SHORT_DOUBLE_PROB, roll);
   
   
   //calc next:
   if(roll[0]==roll[1]){
     roll[0]=1;
     roll[1]++;
   }
   else{
     roll[0]++;
   }
     
   return result;
 
 }
 
 
