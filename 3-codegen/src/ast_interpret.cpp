#include "ast.hpp"

#include <regex>

int32_t Interpret(
    InterpretContext &context, // Contains the parameters and variable bindings
    TreePtr program
){
    std::regex reNum("^-?[0-9]+$");
    std::regex reId("^[a-z][a-z0-9]*$");
    
    if( regex_match(program->type, reNum) ){

        return std::atol(program->type.c_str());
        
    // TODO : Check for things matching reId
        
    }else if(regex_match(program->type, reId) ){
      
        //we have to think of a few cases here, if a variable is mentioned we should return its mapping but if it is assigned or updated this could mean different things...
        std::cout << "value were binding is: " << program->type << std::endl;
        return context.bindings[program->type];

    }else if(program->type=="Param"){
        unsigned index=atol(program->value.c_str());
        auto value=context.params.at(index);
        return value;
        
    }else if(program->type=="Output"){
        int32_t val=Interpret(context, program->branches.at(0));
        std::cout<<val<<std::endl;
        return val;

    // TODO: Handle other constructs
    
    }else if (program->type == "Assign") { //assining the interpert value of the next branch to the variable bindings
        std::cout << "found da assign ting\n" << std::endl;
        int32_t val = Interpret(context, program->branches.at(0));
        context.bindings[program->value] = val;
        return val;

    } else if (program->type == "Seq") {

        for (int i{}; i < program->branches.size(); i++){
            int32_t val = Interpret(context, program->branches.at(i));
        }
        return val;
        //chat says this
        
    } else if (program->type == "LessThan") {

        int32_t l_val = Interpret(context, program->branches.at(0));
        int32_t r_val = Interpret(context, program->branches.at(1));

        if (l_val < r_val) return 1;
        else return 0;

    } else if (program->type == "Add") {

        int32_t l_val = Interpret(context, program->branches.at(0));
        int32_t r_val = Interpret(context, program->branches.at(1));
        
        return l_val + r_val;

    } else if (program->type == "Sub") {

    } else if (program->type == "If") {

    } else if (program->type == "While") {

    } else {
        std::cout << "Unknown construct " << program->type << std::endl;
        throw std::runtime_error("Unknown construct '"+program->type+"'");
    }
}
