#include <adast/semantic/SemanticAnalyzer.hpp>

SemanticAnalyzer::SemanticAnalyzer()
{
    visitor = std::make_unique<SemanticNodeVisitor>();
}

void SemanticAnalyzer::check(AST* _ast)
{
    _ast->accept(visitor.get());
}
