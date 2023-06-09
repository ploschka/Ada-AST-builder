#include <adast/factory/LexerFactory.hpp>
#include <adast/lexer/Lexer.hpp>

std::unique_ptr<LexerInterface> LexerFactory::create()
{
    return std::make_unique<Lexer>();
}
