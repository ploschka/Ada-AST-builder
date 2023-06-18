#pragma once
#include <adast/interface/LexerStateInterface.hpp>
#include <adast/interface/LexerInterface.hpp>
#include <adast/lexer/FileData.hpp>
#include <memory>
#include <stack>

#define st(name)                                         \
    class name : public BaseLexerState                   \
    {                                                    \
    public:                                              \
        name(LexerInterface *_lex, FileData *_filedata); \
        bool recognize(char _c);                         \
    };

typedef std::stack<unsigned int> instack;

class BaseLexerState : public LexerStateInterface
{
protected:
    LexerInterface *lexer;
    FileData *filedata;
    unsigned int initpos;
    Type type;
    BaseLexerState(LexerInterface *_lex, FileData *_filedata);
};

class Id : public BaseLexerState
{
private:
    bool hasUnderscore = false;
public:
    Id(LexerInterface *_lex, FileData *_filedata);
    bool recognize(char _c);
};

st(Start)
st(Skip)
st(FirstNumPart)
st(SecondNumPart)
st(String)
st(Character)
st(Colon)
st(Semicolon)
st(VerticalLine)
st(Dot)
st(Plus)
st(Minus)
st(Star)
st(Div)
st(Ampersand)
st(Greater)
st(Less)
st(Equal)
st(Inv)
st(Band)
st(Lpr)
st(Rpr)
st(Comment)
st(Comma)
st(Newline)

#undef st
