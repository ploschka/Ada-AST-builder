#pragma once
#include <adast/AST/ASTNode.hpp>

class AST {
    BaseASTNode* root;
public:
    AST(BaseASTNode* root);
    void print();
};
