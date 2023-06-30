#pragma once
#include <adast/AST/AST.hpp>

class SemanticAnalyzerInterface
{
public:
    virtual void check(AST *_ast) = 0;
    virtual ~SemanticAnalyzerInterface() = default;
};
