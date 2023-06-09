#include <iostream>
#include <adast/factory/LexerFactory.hpp>
#include <adast/factory/ParserFactory.hpp>

int main()
{
    auto lexfac = LexerFactory();
    auto parfac = ParserFactory();

    auto lexer = lexfac.create();
    auto parser = parfac.create();
    return 0;
}
