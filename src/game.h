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
    Evaluator evaluator;
public:
    game(size_t sampling_number, const std::string & mem_file, size_t number_of_players);
    typedef std::vector<std::string> Command;
    bool execute_command(const Command &command, std::ostream &out);
};


