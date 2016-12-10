#include "game.h"

#include <iostream>

#include "StringUtils.h"
#include "QuixxException.h"

#include "Command.h"

namespace{
    void print_state(const State &state, std::ostream &out){
        for(size_t i=0;i<COLOR_CNT;i++){
            Color color=static_cast<Color>(i);
            ColorState cs=state.get_color_state(color);  
            out<<color2str(color)<<":\t"<<cs.cnt<<" taken, last="<<cs.last<<std::endl;
        }
        out<<"missed: "<<state.get_missed()<<std::endl;
    }
    
    void wrong_take_syntax(std::ostream &out, const game::Command &command){
        out<<"unknown syntax: '"<<stringutils::join(command)<<"'. Known syntax either 'take miss' or 'take <color> <number>'"<<std::endl;
    }
    
    void take_missed(State &state, std::ostream &out, const game::Command &command){
        if(command.at(1)!="miss"){
            wrong_take_syntax(out, command);
            return;
        }
        state.add_miss();
    }
    
    void take(State &state, std::ostream &out, const game::Command &command){
        if(command.size()==2){
          take_missed(state, out, command);
          return;
        }
        //normal take
        if(command.size()!=3){
            wrong_take_syntax(out, command);
            return;
        }
        Color color;
        if(!str2color(command.at(1), color)){
            out << "unknown color '"<<command.at(1)<<"'. Known colors are 'red', 'yellow', 'green', and 'blue'"<<std::endl;
            return;
        }
        int number;
        if(!stringutils::str2int(command.at(2), number)){
            out << "could not convert '"<<command.at(2)<<"' to number"<<std::endl;
            return;
        }
        
        if(!state.take(color, number)){
            out << "invalid move"<<std::endl;
            return;
        }  
    }
    
    void set(State &state, std::ostream &out, const game::Command &command){
       //format of set: <taken_red> <last_red> <taken_yellow> <last_yellow> <taken_green> <last_green> <taken_blue> <last_blue> <missed>
        if(command.size()!=10)
            THROW_QUIXX("invalid syntax: '"<<stringutils::join(command)
                        <<"'. Known syntax is 'set <taken_red> <last_red>"
                          " <taken_yellow> <last_yellow> <taken_green> <last_green>"
                          " <taken_blue> <last_blue> <missed>'");       

       State new_state;
       for(size_t i=0;i<COLOR_CNT;i++){
            Color color=static_cast<Color>(i);
            int taken, last;
            if(!stringutils::str2int(command.at(i*2+1), taken) ||
               !stringutils::str2int(command.at(i*2+2), last)){
               out << "cannot parse for color "<<color2str(color)<<std::endl;
               return;   
            }
            if(!new_state.set(color, last, taken)){
               out << "invalid set for color "<<color2str(color)<<std::endl;
            }
       }
       int missed;
       if(!stringutils::str2int(command.back(), missed)){
            out << "cannot parse missed"<<std::endl;
            return; 
       }
       if(!new_state.set_missed(missed)){
            out << "invalid missed"<<std::endl;
            return;       
       }
       
       //everthing ok:
       state=new_state;
    }

}


bool game::execute_command(const std::vector<std::string> &command, std::ostream &out){

    try{
        if(command.empty())
            return true;
        if(command.at(0)=="exit")
            return false;
        if(command.at(0)=="print"){
            print_state(state, out);
            return true;
        }
        
        if(command.at(0)=="take"){
            take(state, out, command);
            return true;
        }
                        
        if(command.at(0)=="set"){
            set(state, out, command);
            return true;
        }
    }
    catch (QuixxException &ex){
      out << ex.what() << std::endl;
      return true;
    }
    
    try{
        ::Command &c=CommandDictionary::get_command(command.at(0));
        bool return_value=true;
        std::string message=c.check_syntax(command);
        if(message.empty()){
            message=c.check_validity(state, command);
            if(message.empty()){
                message=c.execute(state, command);
                return_value=!c.exit_program();
            }
        }
        out<<message<<std::endl;
        return return_value;
    }       
    catch (QuixxException &ex){
     // out << ex.what() << std::endl;
      out<<"unknown command: "<<command.at(0)<<std::endl;
    }
    return true;
}


