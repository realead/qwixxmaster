#pragma once

#include <array>
#include <random>


typedef std::array<int, 6> DiceRoll;
typedef std::array<int, 2> ShortDiceRoll;


namespace RandomDice{

    //returns 6 values which can be interpreted as red,yellow,green, blue, first white, second white
    DiceRoll random_roll();
}


class DiceRoller{
private:
  std::mt19937 gen; 
  std::uniform_int_distribution<int>  distr;           
public:
  DiceRoller(size_t seed);
  DiceRoll roll();
};






























