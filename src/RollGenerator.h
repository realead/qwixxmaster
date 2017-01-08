#pragma once


#include <array>


typedef std::array<int, 6> DiceRoll;
typedef std::array<int, 2> ShortDiceRoll;


/*!\brief interface for iterating through rolls */


struct RollPair{
    double probability;
    DiceRoll roll;
    RollPair(double prob, const DiceRoll &roll_):
        probability(prob), roll(roll_){}
};

class RollGenerator{

public:
    virtual bool has_next() const=0;
    virtual RollPair get_next()=0;
    virtual ~RollGenerator(){};
};


/*!\brief interface for iterating through short rolls */


struct ShortRollPair{
    double probability;
    ShortDiceRoll roll;
    ShortRollPair(double prob, const ShortDiceRoll &roll_):
        probability(prob), roll(roll_){}
};

class ShortRollGenerator{

public:
    virtual bool has_next() const=0;
    virtual ShortRollPair get_next()=0;
    virtual ~ShortRollGenerator(){};
};




