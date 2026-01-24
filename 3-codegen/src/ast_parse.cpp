#include "ast.hpp"

static std::vector<std::string> Tokenise(std::istream &src)
{
  //put whitespace separated elements into vector res from input stream src
    std::vector<std::string> res;
    std::string tmp;
    while(src >> tmp){
        res.push_back(tmp);
    }
    return res;
}

static std::pair<TreePtr,int> ParseImpl(const std::vector<std::string> &tokens, int pos)
{
    std::string type;
    std::string value;
    std::vector<TreePtr> branches;

    type=tokens.at(pos++);
    
    if(pos < tokens.size()){
        if(tokens.at(pos)==":"){
            pos++;
            
            value=tokens.at(pos++);
        }
    }
    
    if(pos < tokens.size()){
        if(tokens.at(pos)=="["){
            pos++;
            // reach inside a '[' and do a recursive call to add that to the tree and reset pos to past that position
            while(tokens.at(pos)!="]"){
                auto sub=ParseImpl(tokens, pos);
                branches.push_back( sub.first );
                pos=sub.second;
            }
            
            pos++; // skip ']'
        }
    }
    
    return std::make_pair(std::make_shared<Tree>(type, value, branches),pos);
}

TreePtr Parse(std::istream &src)
{
    auto tokens=Tokenise(src);
    return ParseImpl(tokens, 0).first;
}
