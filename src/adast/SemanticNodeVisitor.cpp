#include <adast/semantic/SemanticNodeVisitor.hpp>
#include <adast/AST/ASTNode.hpp>
#include <stdexcept>

void SemanticNodeVisitor::visitLeaf(Leaf *_acceptor) {}
void SemanticNodeVisitor::visitFormalParamsNode(FormalParamsNode *_acceptor) {}

void SemanticNodeVisitor::visitVariableNode(VariableNode *_acceptor)
{
    auto token = _acceptor->chain[0]->token;
    if (token.getType() == Type::id)
    {
        auto symbol = symtable.top()->find(token.getValue());
        if (symbol == symtable.top()->end())
        {
            throw std::runtime_error(
                "Name " + token.getValue() + " is undefined\n" +
                "Occured at row: " +
                std::to_string(token.getRow()) + " position: " + std::to_string(token.getPos()) + "\n");
        }
    }
}

void SemanticNodeVisitor::visitReturnNode(ReturnNode *_acceptor)
{
    _acceptor->return_value->accept(this);
}

void SemanticNodeVisitor::visitCallNode(CallNode *_acceptor)
{
    _acceptor->callable->accept(this);
    _acceptor->params->accept(this);
}

void SemanticNodeVisitor::visitActualParamsNode(ActualParamsNode *_acceptor)
{
    for (auto &i : _acceptor->params)
    {
        i->accept(this);
    }
}

void SemanticNodeVisitor::visitAssignmentNode(AssignmentNode *_acceptor)
{
    auto token = _acceptor->left->token;
    if (token.getType() == Type::id)
    {
        auto symbol = symtable.top()->find(token.getValue());
        if (symbol == symtable.top()->end())
        {
            symtable.top()->insert({token.getValue(), token});
        }
    }
}

void SemanticNodeVisitor::visitBinaryNode(BinaryNode *_acceptor)
{
    _acceptor->left->accept(this);
    _acceptor->right->accept(this);
}

void SemanticNodeVisitor::visitUnaryNode(UnaryNode *_acceptor)
{
    _acceptor->operand->accept(this);
}

void SemanticNodeVisitor::visitBlockNode(BlockNode *_acceptor)
{
    for (auto &i : _acceptor->children)
    {
        i->accept(this);
    }
}

void SemanticNodeVisitor::visitProgramNode(ProgramNode *_acceptor)
{
    for (auto &i : _acceptor->children)
    {
        i->accept(this);
    }
}

void SemanticNodeVisitor::visitFunctionNode(FunctionNode *_acceptor)
{
    auto token = _acceptor->id->token;
    auto symbol = symtable.top()->find(token.getValue());
    if (symbol == symtable.top()->end())
    {
        symtable.top()->insert({token.getValue(), token});
        symtable.push(std::make_unique<localtable_t>(*symtable.top()));
        for (auto &i : _acceptor->formal_params->names)
        {
            symtable.top()->insert({i->token.getValue(), i->token});
        }
        _acceptor->body->accept(this);
        symtable.pop();
    }
    else
    {
        throw std::runtime_error(
            "Name " + token.getValue() + " is already defined\n" +
            "Defined at row: " +
            std::to_string(token.getRow()) + " position: " + std::to_string(token.getPos()) + "\n");
    }
}

void SemanticNodeVisitor::visitProcedureNode(ProcedureNode *_acceptor)
{
    auto token = _acceptor->id->token;
    auto symbol = symtable.top()->find(token.getValue());
    if (symbol == symtable.top()->end())
    {
        symtable.top()->insert({token.getValue(), token});
        symtable.push(std::make_unique<localtable_t>(*symtable.top()));
        _acceptor->body->accept(this);
        symtable.pop();
    }
    else
    {
        throw std::runtime_error(
            "Name " + token.getValue() + " is already defined\n" +
            "Defined at row: " +
            std::to_string(token.getRow()) + " position: " + std::to_string(token.getPos()) + "\n");
    }
}

void SemanticNodeVisitor::visitElseNode(ElseNode *_acceptor)
{
    symtable.push(std::make_unique<localtable_t>(*symtable.top()));
    _acceptor->body->accept(this);
    symtable.pop();
}

void SemanticNodeVisitor::visitElifNode(ElifNode *_acceptor)
{
    _acceptor->condition->accept(this);

    symtable.push(std::make_unique<localtable_t>(*symtable.top()));
    _acceptor->body->accept(this);
    symtable.pop();

    if (_acceptor->next_elif)
    {
        _acceptor->next_elif->accept(this);
    }
    if (_acceptor->next_else)
    {
        _acceptor->next_else->accept(this);
    }
}

void SemanticNodeVisitor::visitIfNode(IfNode *_acceptor)
{
    _acceptor->condition->accept(this);

    symtable.push(std::make_unique<localtable_t>(*symtable.top()));
    _acceptor->body->accept(this);
    symtable.pop();

    if (_acceptor->next_elif)
    {
        _acceptor->next_elif->accept(this);
    }
    if (_acceptor->next_else)
    {
        _acceptor->next_else->accept(this);
    }
}

void SemanticNodeVisitor::visitWhileNode(WhileNode *_acceptor)
{
    _acceptor->condition->accept(this);

    symtable.push(std::make_unique<localtable_t>(*symtable.top()));
    _acceptor->body->accept(this);
    symtable.pop();
}

void SemanticNodeVisitor::visitForNode(ForNode *_acceptor)
{
    auto iter = _acceptor->iterator->token;
    auto from = _acceptor->from->token;
    auto to = _acceptor->to->token;

    if (from.getType() == Type::id)
    {
        auto symbol = symtable.top()->find(from.getValue());
        if (symbol == symtable.top()->end())
        {
            throw std::runtime_error(
                "Name " + from.getValue() + " is undefined\n" +
                "Occured at row: " +
                std::to_string(from.getRow()) + " position: " + std::to_string(from.getPos()) + "\n");
        }
    }
    if (to.getType() == Type::id)
    {
        auto symbol = symtable.top()->find(to.getValue());
        if (symbol == symtable.top()->end())
        {
            throw std::runtime_error(
                "Name " + to.getValue() + " is undefined\n" +
                "Occured at row: " +
                std::to_string(to.getRow()) + " position: " + std::to_string(to.getPos()) + "\n");
        }
    }

    symtable.push(std::make_unique<localtable_t>(*symtable.top()));
    symtable.top()->insert({iter.getValue(), iter});
    _acceptor->body->accept(this);
    symtable.pop();
}

void SemanticNodeVisitor::stdinit(localtable_t* _std)
{
    symtable.emplace(_std);
}
