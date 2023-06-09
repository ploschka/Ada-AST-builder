#include <adast/factory/ParserFactory.hpp>
#include <adast/parser/Parser.hpp>

std::unique_ptr<ParserInterface> ParserFactory::create()
{
    return std::make_unique<Parser>();
}