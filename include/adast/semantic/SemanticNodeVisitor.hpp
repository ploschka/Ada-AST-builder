#pragma once
#include <adast/interface/NodeVisitorInterface.hpp>
#include <adast/token/Token.hpp>
#include <memory>
#include <stack>
#include <string>
#include <utility>
#include <map>

class SemanticNodeVisitor : public NodeVisitorInterface
{
public:
    typedef std::map<std::string, Token> localtable_t;
    void visitLeaf(Leaf *_acceptor);
    void visitFormalParamsNode(FormalParamsNode *_acceptor);
    void visitActualParamsNode(ActualParamsNode *_acceptor);
    void visitVariableNode(VariableNode *_acceptor);
    void visitCallNode(CallNode *_acceptor);
    void visitBinaryNode(BinaryNode *_acceptor);
    void visitUnaryNode(UnaryNode *_acceptor);
    void visitAssignmentNode(AssignmentNode *_acceptor);
    void visitReturnNode(ReturnNode *_acceptor);
    void visitBlockNode(BlockNode *_acceptor);
    void visitProgramNode(ProgramNode *_acceptor);
    void visitFunctionNode(FunctionNode *_acceptor);
    void visitProcedureNode(ProcedureNode *_acceptor);
    void visitElseNode(ElseNode *_acceptor);
    void visitElifNode(ElifNode *_acceptor);
    void visitIfNode(IfNode *_acceptor);
    void visitWhileNode(WhileNode *_acceptor);
    void visitForNode(ForNode *_acceptor);

    void stdinit(localtable_t *);

private:
    std::stack<std::unique_ptr<localtable_t>> symtable;
};
