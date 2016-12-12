#pragma once

#include <vector>

#include "RandomDice.h"

class State;

class Evaluator{
private:
    size_t sampling_number;
    std::vector<float> mem;   
public:
    Evaluator(size_t sampling_number);
    float evaluate_state(const State &state);
    float evaluate_roll(const State &state, const DiceRoll &roll);
    
    typedef std::pair<float, std::string> MoveInfo;
    typedef std::vector<MoveInfo> MoveInfos;
    MoveInfos get_roll_evaluation(const State &state, const DiceRoll &roll);
};


