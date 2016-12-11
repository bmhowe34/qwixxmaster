#pragma once

#include <array>

#include "Command.h"
#include "State.h"

#define CREATE_COMMAND_PARSER_DECLARATION(Name)\
    class Name##CommandParser: public CommandParser{\
    public:\
        virtual CommandExecuterPtr parse(const CommandLine &line);\
        static std::string command_name();\
    };
    
#define CREATE_COMMAND_EXECUTER_DECLARATION(Name)\
    class  Name##CommandExecuter: public CommandExecuter{\
    public: \
         virtual std::string execute(State &state);\
         virtual bool exit_program();\
    };
    
#define CREATE_COMMAND_DECLARATION(Name)\
    CREATE_COMMAND_EXECUTER_DECLARATION(Name);\
    CREATE_COMMAND_PARSER_DECLARATION(Name);

CREATE_COMMAND_DECLARATION(Score);
CREATE_COMMAND_DECLARATION(Exit);
CREATE_COMMAND_DECLARATION(Print);

CREATE_COMMAND_PARSER_DECLARATION(Take);
CREATE_COMMAND_EXECUTER_DECLARATION(TakeMiss);

class  TakeColorCommandExecuter: public CommandExecuter{
     Color color;
     int number;
public: 
     TakeColorCommandExecuter(Color color, int number);
     virtual std::string execute(State &state);
     virtual bool exit_program();
};

CREATE_COMMAND_PARSER_DECLARATION(Set);


class  SetCommandExecuter: public CommandExecuter{
     std::array<int,COLOR_CNT> last;
     std::array<int,COLOR_CNT> taken;
     int missed;
public: 
     SetCommandExecuter(const std::array<int,COLOR_CNT> &last, const std::array<int,COLOR_CNT> &taken, int missed);
     virtual std::string execute(State &state);
     virtual bool exit_program();
};


CREATE_COMMAND_PARSER_DECLARATION(Possible);

class PossibleCommandExecuter: public CommandExecuter{
     Color color;
     int number;
public: 
     PossibleCommandExecuter(Color color, int number);
     virtual std::string execute(State &state);
     virtual bool exit_program();
};


CREATE_COMMAND_PARSER_DECLARATION(Ended);
CREATE_COMMAND_EXECUTER_DECLARATION(Ended);


