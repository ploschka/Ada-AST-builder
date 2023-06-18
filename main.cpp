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

        std::ifstream file(argv[1]);

        auto lexer = lexfac.create();
        lexer->open(file);

        auto parser = parfac.create();

        lexer->print_all_tokens();
//        parser->setLexer(lexer.get());
//        parser->getAST()->print();
    }
    return 0;
}
