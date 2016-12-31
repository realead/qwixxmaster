#pragma once


#include "RollGenerator.h"


class BruteForceRollGenerator : public RollGenerator{
    DiceRoll roll;    
public:
    BruteForceRollGenerator();
    virtual bool has_next() const;
    virtual RollPair get_next();
};

 
