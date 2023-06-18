#include <adast/AST/AST.hpp>

AST::AST(BaseASTNode *root) {
    this->root = root;
}

void AST::print() {
    this->root->print(0);
}
