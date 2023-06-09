#pragma once
#include <adast/interface/LexerInterface.hpp>
#include <memory>

class LexerFactory
{
public:
    std::unique_ptr<LexerInterface> create();
};
