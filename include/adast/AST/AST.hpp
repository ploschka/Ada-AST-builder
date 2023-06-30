#pragma once
#include <adast/AST/ASTNode.hpp>
#include <adast/interface/NodeVisitorInterface.hpp>

class AST
{
    BaseASTNode *root;

public:
    AST(BaseASTNode *root);
    void print();
    void accept(NodeVisitorInterface *_visitor);
};
