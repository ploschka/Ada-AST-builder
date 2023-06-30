#include <adast/semantic/SemanticAnalyzer.hpp>

SemanticAnalyzer::SemanticAnalyzer()
{
    visitor = std::make_unique<SemanticNodeVisitor>();
}

void SemanticAnalyzer::check(AST* _ast)
{
    _ast->accept(visitor.get());
}

void SemanticAnalyzer::stdinit()
{
    auto tr = Token("true", Type::id);
    auto fl = Token("false", Type::id);

    std::unique_ptr<SemanticNodeVisitor::localtable_t> stdlib = std::make_unique<SemanticNodeVisitor::localtable_t>();

    stdlib->insert({tr.getValue(), tr});
    stdlib->insert({fl.getValue(), fl});

    visitor->stdinit(stdlib.release());
}
