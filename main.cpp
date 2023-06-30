#include <iostream>
#include <fstream>
#include <filesystem>
#include <adast/factory/LexerFactory.hpp>
#include <adast/factory/ParserFactory.hpp>
#include <adast/factory/SemanticFactory.hpp>

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
        auto semfac = SemanticFactory();


        auto lexer = lexfac.create();
        auto parser = parfac.create();
        auto seman = semfac.create();

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
        auto ast = parser->getAST();
        ast->print();
        
        // Инициализируем стандартную библитеку
        seman->stdinit();
        
        // Проводим семантический анализ дерева
        seman->check(ast);        
    }
    return 0;
}
