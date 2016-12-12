#pragma once

#include <vector>

#include "RandomDice.h"

class State;

class Evaluator{
private:
    std::vector<float> mem;   
public:
    Evaluator();
    float evaluate_state(const State &state);
    float evaluate_roll(const State &state, const DiceRoll &roll);
};


