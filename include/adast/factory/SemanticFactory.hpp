#pragma once
#include <adast/interface/SemanticAnalyzerInterface.hpp>
#include <memory>

class SemanticFactory
{
public:
    std::unique_ptr<SemanticAnalyzerInterface> create();
};
