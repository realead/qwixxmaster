#pragma once

#include <array>



typedef std::array<int, 6> DiceRoll;


namespace RandomDice{

    //returns 6 values which can be interpreted as red,yellow,green, blue, first white, second white
    DiceRoll random_roll();
}


