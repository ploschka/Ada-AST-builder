#include <adast/factory/SemanticFactory.hpp>
#include <adast/semantic/SemanticAnalyzer.hpp>

std::unique_ptr<SemanticAnalyzerInterface> SemanticFactory::create()
{
    return std::make_unique<SemanticAnalyzer>();
}
