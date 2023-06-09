#pragma once
#include <adast/token/Token.hpp>
#include <adast/interface/LexerStateInterface.hpp>

class LexerInterface
{
public:
    virtual void open(std::istream& _stream) = 0;
    virtual Token getToken() = 0;
    virtual void setState(LexerStateInterface *_state) = 0;
    virtual void print_all_tokens() = 0;
    virtual ~LexerInterface() = default;
};
