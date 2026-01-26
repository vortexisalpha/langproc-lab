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

        CompileRec(destReg, program->branches.at(0));
        std::cout << "output "<< destReg <<std::endl; // value
                                         //
    } else if (program->type == "Assign"){
        
        std::string var = program->value;
        CompileRec(var, program->branches.at(0));

        std::string zero = makeName("zero");
        std::cout << "const " << zero << " 0" << std::endl;

        std::cout << "add " << destReg << " "  << var << " " << zero << std::endl;

    } else if (program->type == "Input"){
        
        std::cout << "input " << destReg << std::endl;

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

        std::string left = makeName("left");
        std::string right = makeName("right");
        
        CompileRec(left, program->branches.at(0));
        CompileRec(right, program->branches.at(1));

        std::cout << "lt " << destReg << " "  << left << " " << right << std::endl;

    } else if (program->type == "If"){
       
        std::string cond = makeName("cond");
        std::string cond_label = makeName("cond_label");
        std::string end_label = makeName("end");

        std::string zero = makeName("zero");
        std::cout << "const " << zero << " 0" << std::endl;

        CompileRec(cond, program->branches.at(0));

        std::cout << "beq " << cond << " " << zero << " " << cond_label << std::endl;

        CompileRec(destReg, program->branches.at(1));
        
        std::cout << "beq " << zero << " "<< zero << " " << end_label << std::endl;

        std::cout << ":" << cond_label << std::endl;
        CompileRec(destReg, program->branches.at(2));
        std::cout << ":" << end_label << std::endl;
         

    } else if (program->type == "While"){

        std::string cond = makeName("cond");
        std::string while_label = makeName("while");
        std::string end_label = makeName("end");
        
        std::string zero = makeName("zero");
        std::cout << "const " << zero << " 0" << std::endl;
        std::cout << ":" << while_label << std::endl;

        CompileRec(cond, program->branches.at(0));

        std::cout << "beq " << cond << " " << zero << " " << end_label << std::endl;;
        CompileRec(destReg, program->branches.at(1));

        std::cout << "beq " << zero << " " << zero << " " << while_label << std::endl;;

        std::cout << ":" << end_label << std::endl;
        std::cout << "const " << destReg << " 0" << std::endl;
        
    } else {
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
