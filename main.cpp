#include <iostream>
#include <fstream>
#include <filesystem>
#include <adast/factory/LexerFactory.hpp>
#include <unordered_map>
// #include <adast/factory/ParserFactory.hpp>

static std::unordered_map<Type, std::string> map = {
    {Type::id, "id"},
    {Type::number, "number"},
    {Type::string, "string"},
    {Type::character, "character"},
    {Type::lpr, "lpr"},
    {Type::rpr, "rpr"},
    {Type::colon, "colon"},
    {Type::semicolon, "semicolon"},
    {Type::ampersand, "ampersand"},
    {Type::box, "box"},
    {Type::llabbr, "llabbr"},
    {Type::rlabbr, "rlabbr"},
    {Type::plus, "plus"},
    {Type::minus, "minus"},
    {Type::star, "star"},
    {Type::power, "power"},
    {Type::div, "div"},
    {Type::dot, "dot"},
    {Type::doubledot, "doubledot"},
    {Type::greater, "greater"},
    {Type::less, "less"},
    {Type::equal, "equal"},
    {Type::noteq, "noteq"},
    {Type::grequal, "grequal"},
    {Type::lequal, "lequal"},
    {Type::mod, "mod"},
    {Type::notop, "notop"},
    {Type::in, "in"},
    {Type::is, "is"},
    {Type::andop, "andop"},
    {Type::orop, "orop"},
    {Type::xorop, "xorop"},
    {Type::vertical, "vertical"},
    {Type::assign, "assign"},
    {Type::arrow, "arrow"},
    {Type::abortkw, "abortkw"},
    {Type::abskw, "abskw"},
    {Type::abstractkw, "abstractkw"},
    {Type::acceptkw, "acceptkw"},
    {Type::accesskw, "accesskw"},
    {Type::aliasedkw, "aliasedkw"},
    {Type::allkw, "allkw"},
    {Type::arraykw, "arraykw"},
    {Type::atkw, "atkw"},
    {Type::beginkw, "beginkw"},
    {Type::bodykw, "bodykw"},
    {Type::casekw, "casekw"},
    {Type::constantkw, "constantkw"},
    {Type::declarekw, "declarekw"},
    {Type::delaykw, "delaykw"},
    {Type::deltakw, "deltakw"},
    {Type::digitskw, "digitskw"},
    {Type::dokw, "dokw"},
    {Type::endkw, "endkw"},
    {Type::entrykw, "entrykw"},
    {Type::exceptionkw, "exceptionkw"},
    {Type::exitkw, "exitkw"},
    {Type::functionkw, "functionkw"},
    {Type::generickw, "generickw"},
    {Type::gotokw, "gotokw"},
    {Type::interfacekw, "interfacekw"},
    {Type::limitedkw, "limitedkw"},
    {Type::loopkw, "loopkw"},
    {Type::newkw, "newkw"},
    {Type::nullkw, "nullkw"},
    {Type::ofkw, "ofkw"},
    {Type::otherskw, "otherskw"},
    {Type::outkw, "outkw"},
    {Type::overridkw, "overridkw"},
    {Type::packagekw, "packagekw"},
    {Type::pragmakw, "pragmakw"},
    {Type::privatekw, "privatekw"},
    {Type::procedurekw, "procedurekw"},
    {Type::protectedkw, "protectedkw"},
    {Type::raisekw, "raisekw"},
    {Type::rangekw, "rangekw"},
    {Type::recordkw, "recordkw"},
    {Type::remkw, "remkw"},
    {Type::renameskw, "renameskw"},
    {Type::requeuekw, "requeuekw"},
    {Type::reversekw, "reversekw"},
    {Type::selectkw, "selectkw"},
    {Type::separatekw, "separatekw"},
    {Type::somekw, "somekw"},
    {Type::subtypekw, "subtypekw"},
    {Type::synckw, "synckw"},
    {Type::taggedkw, "taggedkw"},
    {Type::taskkw, "taskkw"},
    {Type::terminatekw, "terminatekw"},
    {Type::thenkw, "thenkw"},
    {Type::typekw, "typekw"},
    {Type::untilkw, "untilkw"},
    {Type::usekw, "usekw"},
    {Type::whenkw, "whenkw"},
    {Type::withkw, "withkw"},
    {Type::ifkw, "ifkw"},
    {Type::elsifkw, "elsifkw"},
    {Type::elsekw, "elsekw"},
    {Type::forkw, "forkw"},
    {Type::whilekw, "whilekw"},
    {Type::returnkw, "returnkw"},
    {Type::eof, "eof"},
    {Type::comma, "comma"},
    {Type::unexpected, "unexpected"}};

int main(int argc, char *argv[])
{
    if (argc > 1)
    {

        if (!std::filesystem::exists(argv[1]))
        {
            std::cerr << "File " << argv[1] << " does not exist!\n";
            return -1;
        }
        if (std::filesystem::is_directory(argv[1]))
        {
            std::cerr << argv[1] << " is a directory!\n";
            return -1;
        }

        auto lexfac = LexerFactory();
        // auto parfac = ParserFactory();

        std::ifstream file(argv[1]);

        auto lexer = lexfac.create();
        lexer->open(file);
        
        bool keepGoing = true;
        while (keepGoing)
        {
            auto lexeme = lexer->getToken();
            keepGoing = lexeme.getType() != Type::eof;
            std::cout << '"' << lexeme.getValue() << "\"  " << map.at(lexeme.getType()) << " row: " << lexeme.getRow() << " pos: " << lexeme.getPos() << std::endl;
        }

        // auto parser = parfac.create();
    }
    return 0;
}
