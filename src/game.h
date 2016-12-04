#pragma once

#include <vector>
#include <string>
#include <iosfwd>

#include "State.h"

class game{
private:
    State state;   
public:
    typedef std::vector<std::string> Command;
    bool execute_command(const Command &command, std::ostream &out);
};


