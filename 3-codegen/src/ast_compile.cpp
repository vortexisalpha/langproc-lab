#include "ast.hpp"

#include <string>
#include <regex>

static int makeNameUnq=0;

static std::string makeName(std::string base)
{
    return "_"+base+"_"+std::to_string(makeNameUnq++);
}

void CompileRec(
    std::string destReg,    // The name of the register to put the result in
    TreePtr program
){
    std::regex reNum("^-?[0-9]+$");
    std::regex reId("^[a-z][a-z0-9]*$");
    
    if( regex_match( program->type, reNum ) ){

        std::cout << "const " << destReg << " " << program->type << std::endl;
        
    } else if( regex_match( program->type, reId ) ){
        
        std::string zero = makeName("zero"); // string _zero_{i} where i is the index of the register?
        std::cout << "const " << zero << " 0" << std::endl;
        std::cout << "add " << destReg << " " << program->type << " " << zero << std::endl;

    } else if(program->type=="Param"){

        std::cout << "param " << destReg << " " << program->value << std::endl;

    } else if(program->type=="Seq"){

        for(unsigned i=0; i<program->branches.size(); i++){
            CompileRec(destReg, program->branches[i]);
        } 
    }
    // TODO : handle the others
    else if (program->type == "Output"){

//is this needed?
        CompileRec(destReg, program->branches.at(0));
        std::cout<< destReg <<std::endl; // value
                                         //
    } else if (program->type == "Assign"){
        
        std::string var = makeName(program->value);
        CompileRec(var, program->branches.at(0));

    } else if (program->type == "Input"){

    } else if (program->type == "Add"){

        std::string left = makeName("left");
        std::string right = makeName("right");
        
        CompileRec(left, program->branches.at(0));
        CompileRec(right, program->branches.at(1));

        std::cout << "add " << destReg << " "  << left << " " << right << std::endl;

    } else if (program->type == "Sub"){

        std::string left = makeName("left");
        std::string right = makeName("right");
        
        CompileRec(left, program->branches.at(0));
        CompileRec(right, program->branches.at(1));

        std::cout << "sub " << destReg << " "  << left << " " << right << std::endl;

    } else if (program->type == "LessThan"){

    } else if (program->type == "If"){

    } else if (program->type == "While"){

    }else 
        throw std::runtime_error("Unknown construct '"+program->type+"'");
    }
}

void Compile(
    TreePtr program
){
    // Create a register to hold the final result
    std::string res=makeName("res");
    
    // Compile the whole thing
    CompileRec(res, program);
    
    // Put the result out
    std::cout << "halt " << res << "\n";
}
