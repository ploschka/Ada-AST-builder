#pragma once
#include <adast/interface/ParserInterface.hpp>
#include <memory>

class ParserFactory
{
public:
    std::unique_ptr<ParserInterface> create();
};
