#pragma once
#include <adast/interface/ASTNodeInterface.hpp>
#include <adast/token/Token.hpp>
#include <vector>

class BaseASTNode : public ASTNodeInterface
{
};

// ============= Expressions =============
class ExpressionNode : public BaseASTNode {
};  // Абстрактный класс выражения

class Leaf : public ExpressionNode {
public:
    void print(int indent) override;
    Token token;
    Leaf(Token token);
};


// ============= Params =============
class FormalParamsNode : public BaseASTNode {
public:
    void print(int indent) override;
    std::vector<Leaf*> names;
    std::vector<Leaf*> types;
    FormalParamsNode(std::vector<Leaf*> params, std::vector<Leaf*> types);
    void add_param(Leaf *name, Leaf *type);
};

class ActualParamsNode : public BaseASTNode {
public:
    void print(int indent) override;
    std::vector<ExpressionNode*> params;
    ActualParamsNode(std::vector<ExpressionNode*> params);
    void add_child(ExpressionNode* param);
};


// ============= Expressions =============
class VariableNode: public ExpressionNode {
public:
    void print(int indent) override;
    std::vector<Leaf*> chain;  // Цепочка обращений. Например: ID.ID.ID, либо ID, либо NUMBER
    VariableNode(std::vector<Leaf*> chain);
    void add_to_chain(Leaf * leaf);
};

class CallNode : public ExpressionNode {
public:
    void print(int indent) override;
    VariableNode* callable;
    ActualParamsNode* params;
    CallNode(VariableNode* callable, ActualParamsNode* params);
};
class BinaryNode : public ExpressionNode {
public:
    void print(int indent) override;
    ExpressionNode* left;
    Leaf* op;
    ExpressionNode* right;
    BinaryNode(ExpressionNode* left, Leaf *op, ExpressionNode* right);
};

class UnaryNode : public ExpressionNode {
public:
    void print(int indent) override;
    Leaf* op;
    ExpressionNode* operand;
    UnaryNode(Leaf *op, ExpressionNode* operand);
};

class AssignmentNode : public BaseASTNode {
public:
    void print(int indent) override;
    Leaf* left;
    ExpressionNode* right;
    AssignmentNode(Leaf* left, ExpressionNode* right);
};

class ReturnNode : public BaseASTNode {
public:
    void print(int indent) override;
    ExpressionNode* return_value;
    ReturnNode(ExpressionNode* return_value);
};


// ============= Block =============

class BlockNode : public BaseASTNode {
public:
    void print(int indent) override;
    std::vector<BaseASTNode*> children;
    BlockNode(std::vector<BaseASTNode*> children);
    BlockNode();

    void add_child(BaseASTNode* child);
};

class ProgramNode : public BlockNode {
    void print(int indent) override;
};

// ============= Compound statements =============

class FunctionNode : public BaseASTNode {
public:
    void print(int indent) override;
    Leaf* id;
    Leaf* return_type;
    FormalParamsNode* formal_params;
    BlockNode* body;
    FunctionNode(Leaf* id, FormalParamsNode* formal_params, Leaf* return_type, BlockNode* body);
};

class ProcedureNode : public BaseASTNode {
public:
    void print(int indent) override;
    Leaf* id;
    FormalParamsNode* formal_params;
    BlockNode* body;
    ProcedureNode(Leaf* id, FormalParamsNode* formal_params, BlockNode* body);
};

class ElseNode : public BaseASTNode {
public:
    void print(int indent) override;
    BlockNode* body;
    ElseNode(BlockNode *body);
};

class ElifNode : public BaseASTNode {
public:
    void print(int indent) override;
    ExpressionNode* condition;
    BlockNode* body;
    ElifNode* next_elif;
    ElseNode* next_else;
    ElifNode(ExpressionNode* condition, BlockNode* body);
};

class IfNode : public BaseASTNode {
public:
    void print(int indent) override;
    ExpressionNode* condition;
    BlockNode* body;
    ElifNode* next_elif;
    ElseNode* next_else;
    IfNode(ExpressionNode* condition, BlockNode* body);
};


class WhileNode : public BaseASTNode {
public:
    void print(int indent) override;
    ExpressionNode* condition;
    BlockNode* body;
    WhileNode(ExpressionNode* condition, BlockNode* body);
};

class ForNode : public BaseASTNode {
public:
    void print(int indent) override;
    Leaf* iterator;
    Leaf* from;
    Leaf* to;
    BlockNode* body;
    ForNode(Leaf *iterator, Leaf *from, Leaf *to, BlockNode *body);
};

void print_indented_line(std::string text, int indent);