#pragma once

#include <vector>
#include <string>
#include <iosfwd>
#include <memory>

#include "State.h"
#include "Evaluator.h"

class game{
private:
    State state;  
    std::unique_ptr<Evaluator> evaluator;
    size_t sampling_number;
public:
    game(size_t sampling_number);
    typedef std::vector<std::string> Command;
    bool execute_command(const Command &command, std::ostream &out);
};


