#include <iostream>
#include <fstream>
#include <filesystem>
#include <adast/factory/LexerFactory.hpp>
#include <adast/factory/ParserFactory.hpp>

int main(int argc, char *argv[]) {
    if (argc > 1) {

        if (!std::filesystem::exists(argv[1])) {
            std::cerr << "File " << argv[1] << " does not exist!\n";
            return -1;
        }
        if (std::filesystem::is_directory(argv[1])) {
            std::cerr << argv[1] << " is a directory!\n";
            return -1;
        }

        auto lexfac = LexerFactory();
        auto parfac = ParserFactory();


        auto lexer = lexfac.create();
        auto parser = parfac.create();

        // Выводим все лексемы, полученные лексером
        std::cout << "Lexer:\n";
        std::ifstream file_lexer(argv[1]);
        lexer->open(file_lexer);
        lexer->print_all_tokens();

        // Выводим дерево, полученное парсером
        std::cout << "\n\nParser:\n";
        std::ifstream file_parser(argv[1]);
        lexer->open(file_parser);
        parser->setLexer(lexer.get());
        parser->getAST()->print();
    }
    return 0;
}
