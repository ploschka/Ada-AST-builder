#pragma once
#include <adast/interface/LexerInterface.hpp>
#include <adast/AST/AST.hpp>

class ParserInterface
{
public:
    virtual void setLexer(LexerInterface*) = 0;
    virtual AST* getAST() = 0;
    virtual ~ParserInterface() = default;
};
