#pragma once

#include <vector>
#include <string>
#include <iosfwd>

#include "State.h"

class game{
private:
    State state;   
public:
    bool execute_command(const std::vector<std::string> &command, std::ostream &out);
};


