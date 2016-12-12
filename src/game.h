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
public:
    typedef std::vector<std::string> Command;
    bool execute_command(const Command &command, std::ostream &out);
};


