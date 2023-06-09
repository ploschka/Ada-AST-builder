#pragma once
#include <adast/interface/ParserInterface.hpp>

class Parser : public ParserInterface
{
public:
    void setLexer(LexerInterface*);
    AST* getAST();
};
