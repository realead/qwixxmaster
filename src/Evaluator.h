#pragma once

#include <vector>
#include <string>
#include <utility>

#include "RandomDice.h"

class State;

class Evaluator{
private:
    size_t sampling_number;
    size_t player_number;
    std::vector<float> mem;
    
    float evaluate_without_whites(const State &state, const DiceRoll &roll);    
public:
    Evaluator(size_t sampling_number, size_t player_number=1);
    float evaluate_state(const State &state);
    
    float evaluate_roll(const State &state, const DiceRoll &roll);
    float evaluate_roll(const State &state, const ShortDiceRoll &roll);
    
    typedef std::pair<float, std::string> MoveInfo;
    typedef std::vector<MoveInfo> MoveInfos;
    MoveInfos get_roll_evaluation(const State &state, const DiceRoll &roll);
    MoveInfos get_roll_evaluation(const State &state, const ShortDiceRoll &roll);
    
    
    //returns: size of the memory, number of unset states
    std::pair<size_t, size_t> save_memory_to_file(const std::string &filename) const;
    std::pair<size_t, size_t> load_memory_from_file(const std::string &filename);
private:  
    void evaluate_without_whites(const State &state, const DiceRoll &roll, MoveInfos &res, const std::string &prefix); 
};


