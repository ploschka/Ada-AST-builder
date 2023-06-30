#pragma once

#include <adast/interface/SemanticAnalyzerInterface.hpp>
#include <adast/semantic/SemanticNodeVisitor.hpp>
#include <memory>

class SemanticAnalyzer: public SemanticAnalyzerInterface
{
private:
    std::unique_ptr<SemanticNodeVisitor> visitor;

public:
    SemanticAnalyzer();
    void check(AST *_ast);
    void stdinit();
};
