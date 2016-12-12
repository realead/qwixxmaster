#pragma once

#include <array>



typedef std::array<int, 6> DiceRoll;
typedef std::array<int, 2> ShortDiceRoll;


namespace RandomDice{

    //returns 6 values which can be interpreted as red,yellow,green, blue, first white, second white
    DiceRoll random_roll();
}


