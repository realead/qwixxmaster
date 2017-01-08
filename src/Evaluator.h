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
       
    size_t get_next_player(size_t current_player) const;
public:
    Evaluator(size_t sampling_number, size_t player_number);
    float evaluate_state(const State &state, size_t current_player);
    
    float evaluate_roll(const State &state, const DiceRoll &roll, size_t current_player);
    float evaluate_roll(const State &state, const ShortDiceRoll &roll, size_t current_player);
    
    typedef std::pair<float, std::string> MoveInfo;
    typedef std::vector<MoveInfo> MoveInfos;
    MoveInfos get_roll_evaluation(const State &state, const DiceRoll &roll, size_t current_player);
    MoveInfos get_roll_evaluation(const State &state, const ShortDiceRoll &roll, size_t current_player);
    
    
    //returns: size of the memory, number of unset states
    std::pair<size_t, size_t> save_memory_to_file(const std::string &filename) const;
    std::pair<size_t, size_t> load_memory_from_file(const std::string &filename);
    
private:  
    float evaluate_without_whites(const State &state, const DiceRoll &roll, size_t current_player); 
    void evaluate_without_whites(const State &state, const DiceRoll &roll, MoveInfos &res, const std::string &prefix, size_t current_player); 
};


