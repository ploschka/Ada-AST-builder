#include <adast/parser/Parser.hpp>
#include <adast/token/Token.hpp>
#include <adast/AST/ASTNode.hpp>
#include <adast/AST/AST.hpp>
#include <stdexcept>
#include <iostream>
#include <map>
#include <set>

// Первые терминалы, которые можно встретить, переходя вглубь нетерминалов грамматики
std::map<std::string, std::set<Type>> FIRSTS = {{"assignment",            {Type::id}},
                                                {"expression",            {Type::id,          Type::lpr,    Type::minus,    Type::plus,     Type::number,  Type::string, Type::notop,  Type::dot}},
                                                {"root_stmt",             {Type::procedurekw, Type::functionkw}},
                                                {"formal_params",         {Type::id}},
                                                {"if_stmt",               {Type::ifkw}},
                                                {"elsif_stmt",            {Type::elsifkw}},
                                                {"statements",            {Type::ifkw,        Type::id,     Type::lpr,      Type::returnkw, Type::whilekw, Type::minus,  Type::plus,   Type::number, Type::notop,  Type::string, Type::dot,    Type::forkw, Type::procedurekw, Type::functionkw}},
                                                {"term",                  {Type::id,          Type::lpr,    Type::minus,    Type::plus,     Type::number,  Type::string, Type::dot}},
                                                {"function_declaration",  {Type::functionkw}},
                                                {"primary",               {Type::id,          Type::lpr,    Type::number,   Type::string,   Type::dot}},
                                                {"actual_params",         {Type::id,          Type::lpr,    Type::minus,    Type::plus,     Type::number,  Type::string, Type::notop,  Type::dot}},
                                                {"for_stmt",              {Type::forkw}},
                                                {"atom",                  {Type::number,      Type::string, Type::id}},
                                                {"func_call",             {Type::lpr}},
                                                {"block",                 {Type::ifkw,        Type::id,     Type::lpr,      Type::returnkw, Type::whilekw, Type::minus,  Type::plus,   Type::number, Type::string, Type::notop,  Type::dot,    Type::forkw}},
                                                {"else_block",            {Type::elsekw}},
                                                {"compound_stmt",         {Type::ifkw,        Type::forkw,  Type::whilekw}},
                                                {"sum",                   {Type::id,          Type::lpr,    Type::minus,    Type::plus,     Type::number,  Type::string, Type::dot}},
                                                {"procedure_declaration", {Type::procedurekw}},
                                                {"nested_stmt",           {Type::ifkw,        Type::id,     Type::lpr,      Type::returnkw, Type::whilekw, Type::minus,  Type::plus,   Type::number, Type::string, Type::notop,  Type::dot,    Type::forkw}},
                                                {"disjunction",           {Type::id,          Type::lpr,    Type::minus,    Type::plus,     Type::number,  Type::string, Type::notop,  Type::dot}},
                                                {"simple_stmt",           {Type::id,          Type::lpr,    Type::returnkw, Type::minus,    Type::plus,    Type::number, Type::string, Type::notop,  Type::dot}},
                                                {"factor",                {Type::id,          Type::lpr,    Type::minus,    Type::plus,     Type::number,  Type::string, Type::dot}},
                                                {"comparison",            {Type::id,          Type::lpr,    Type::minus,    Type::plus,     Type::number,  Type::string, Type::dot}},
                                                {"inversion",             {Type::id,          Type::lpr,    Type::minus,    Type::plus,     Type::number,  Type::string, Type::notop,  Type::dot}},
                                                {"statement",             {Type::ifkw,        Type::id,     Type::lpr,      Type::returnkw, Type::whilekw, Type::minus,  Type::plus,   Type::number, Type::notop,  Type::string, Type::dot,    Type::forkw, Type::procedurekw, Type::functionkw}},
                                                {"return_stmt",           {Type::returnkw}},
                                                {"while_stmt",            {Type::whilekw}},
                                                {"conjunction",           {Type::id,          Type::lpr,    Type::minus,    Type::plus,     Type::number,  Type::string, Type::notop,  Type::dot}},
                                                {"program",               {Type::ifkw,        Type::id,     Type::lpr,      Type::returnkw, Type::whilekw, Type::eof,    Type::minus,  Type::plus,   Type::number, Type::notop,  Type::string, Type::dot,   Type::forkw,       Type::procedurekw, Type::functionkw}}};

Parser::Parser() : token(Token("", Type::id)) {
};

void Parser::setLexer(LexerInterface *lexer) {
    this->lexer = lexer;
    this->next_token();
}

AST *Parser::getAST() {
    return new AST(this->program());
}


bool Parser::is_token_in_firsts(std::string grammar_node) {
    // Возвращает True, если текущий токен кода можно получить, спускаясь по узлу грамматика grammar_node
    return FIRSTS[grammar_node].find(this->token.getType()) != FIRSTS[grammar_node].end();
}

bool Parser::token_matches_any(std::vector<Type> types) {
    // Возвращает True, если текущий токен кода имеет тип, имеющийся в types
    for (Type type: types) {
        if (this->get_token().getType() == type) {
            return true;
        }
    }
    return false;
}

void Parser::error(std::string message) {
    std::string pos = std::to_string(this->get_token().getPos());
    std::string row = std::to_string(this->get_token().getRow());
    std::string value = this->get_token().getValue();
    std::string type = type_to_str(this->get_token().getType());
    message += " pos=" + pos + " row=" + row + " type=" + type + " value=" + value;
    std::cout << message << std::endl;
    throw std::runtime_error(message);
}

void Parser::next_token() {
    if (this->future_tokens.empty()) {
        this->token = this->lexer->getToken();
    } else {
        // Забираем токен из списка будущих токенов
        this->token = this->future_tokens[0];
        this->future_tokens.erase(this->future_tokens.begin());
    }
}

Token Parser::forward(int k) {
    while (this->future_tokens.size() < k) {
        this->future_tokens.push_back(this->lexer->getToken());
    }
    return this->future_tokens[k - 1];
}

Token Parser::get_token() {
    return this->token;
}

Token Parser::check_get_next(Type type) {
    // Проверяет, что текущий токен имеет тип type, возвращает его и сразу получает следующий токен
    if (this->token.getType() != type) {
        this->error("Unexpected token");
    }
    Token tmp = this->token;
    this->next_token();
    return tmp;
}

ProgramNode *Parser::program() {
    /*
    program:
        | statements EOF
        | EOF
     */
    ProgramNode *file = new ProgramNode();
    if (this->is_token_in_firsts("statements")) {
        this->statements(file);
    }
    file->add_child(new Leaf(this->check_get_next(Type::eof)));
    return file;
};

void Parser::statements(BlockNode *parent_block) {
    /*
    statements:
        | statement statements
        | statement
     */
    while (this->is_token_in_firsts("statements")) {
        this->statement(parent_block);
    }
};

void Parser::statement(BlockNode *parent_block) {
    /*
    statement:
        | root_stmt
        | nested_stmt
     */
    if (this->is_token_in_firsts("root_stmt")) {
        this->root_stmt(parent_block);
    } else if (this->is_token_in_firsts("nested_stmt")) {
        this->nested_stmt(parent_block);
    } else {
        this->error("statement");
    }
}

void Parser::root_stmt(BlockNode *parent_block) {
    /*
    root_stmt:
        | function_declaration SEMICOLON
        | procedure_declaration SEMICOLON
     */
    if (this->is_token_in_firsts("function_declaration")) {
        parent_block->add_child(this->function_declaration());
    } else if (this->is_token_in_firsts("procedure_declaration")) {
        parent_block->add_child(this->procedure_declaration());
    } else {
        this->error("statement");
    }
    this->check_get_next(Type::semicolon);
}

FunctionNode *Parser::function_declaration() {
    /*
    function_declaration:
        | FUNCTIONKW ID LPR formal_params RPR RETURNKW ID IS BEGINKW block ENDKW ID
        | FUNCTIONKW ID LPR RPR RETURNKW ID IS BEGINKW block ENDKW ID
     */
    this->check_get_next(Type::functionkw);
    Leaf *id = new Leaf(this->check_get_next(Type::id));
    this->check_get_next(Type::lpr);
    FormalParamsNode *formal_params;
    if (this->is_token_in_firsts("formal_params")) {
        formal_params = this->formal_params();
    } else {
        formal_params = new FormalParamsNode({}, {});
    }
    this->check_get_next(Type::rpr);
    this->check_get_next(Type::returnkw);
    Leaf *return_type = new Leaf(this->check_get_next(Type::id));
    this->check_get_next(Type::is);
    this->check_get_next(Type::beginkw);
    BlockNode *body = this->block();
    this->check_get_next(Type::endkw);
    this->check_get_next(Type::id);
    return new FunctionNode(id, formal_params, return_type, body);
};

ProcedureNode *Parser::procedure_declaration() {
    /*
    procedure_declaration:
        | PROCEDUREKW ID LPR formal_params RPR IS BEGINKW block ENDKW ID
        | PROCEDUREKW ID LPR RPR IS BEGINKW block ENDKW ID
     */
    this->check_get_next(Type::procedurekw);
    Leaf *id = new Leaf(this->check_get_next(Type::id));
    this->check_get_next(Type::lpr);
    FormalParamsNode *formal_params;
    if (this->is_token_in_firsts("formal_params")) {
        formal_params = this->formal_params();
    } else {
        formal_params = new FormalParamsNode({}, {});
    }
    this->check_get_next(Type::rpr);
    this->check_get_next(Type::is);
    this->check_get_next(Type::beginkw);
    BlockNode *body = this->block();
    this->check_get_next(Type::endkw);
    this->check_get_next(Type::id);
    return new ProcedureNode(id, formal_params, body);
}

FormalParamsNode *Parser::formal_params() {
    /*
    formal_params:
        | ID COLON ID SEMICOLON formal_params
        | ID COLON ID
     */
    FormalParamsNode *params = new FormalParamsNode({}, {});
    while (this->is_token_in_firsts("formal_params")) {
        Leaf *name = new Leaf(this->check_get_next(Type::id));
        this->check_get_next(Type::colon);
        Leaf *type = new Leaf(this->check_get_next(Type::id));
        params->add_param(name, type);
        if (this->get_token().getType() == Type::semicolon) {
            this->next_token();
        }
    }
    return params;
};

BlockNode *Parser::block() {
    /*
    block:
        | nested_stmt block
        | nested_stmt
     */
    BlockNode *block = new BlockNode();
    while (this->is_token_in_firsts("block")) {
        this->nested_stmt(block);
    }
    return block;
}

void Parser::nested_stmt(BlockNode *parent_block) {
    /*
    nested_stmt:
        | compound_stmt SEMICOLON
        | simple_stmt SEMICOLON
     */
    if (this->is_token_in_firsts("compound_stmt")) {
        this->compound_stmt(parent_block);
    } else if (this->is_token_in_firsts("simple_stmt")) {
        this->simple_stmt(parent_block);
    }
    this->check_get_next(Type::semicolon);
}

void Parser::compound_stmt(BlockNode *parent_block) {
    /*
    compound_stmt:
        | if_stmt
        | for_stmt
        | while_stmt
     */
    if (this->is_token_in_firsts("if_stmt")) {
        parent_block->add_child(this->if_stmt());
    } else if (this->is_token_in_firsts("for_stmt")) {
        parent_block->add_child(this->for_stmt());
    } else if (this->is_token_in_firsts("while_stmt")) {
        parent_block->add_child(this->while_stmt());
    } else {
        this->error("compound_stmt");
    }
};

IfNode *Parser::if_stmt() {
    /*
    if_stmt:
        | IFKW expression THENKW block elsif_stmt ENDKW IFKW
        | IFKW expression THENKW block else_block ENDKW IFKW
        | IFKW expression THENKW block ENDKW IFKW
     */
    this->check_get_next(Type::ifkw);
    ExpressionNode *condition = this->expression();
    this->check_get_next(Type::thenkw);
    BlockNode *body = this->block();
    IfNode *if_node = new IfNode(condition, body);
    if (this->is_token_in_firsts("elsif_stmt")) {
        if_node->next_elif = this->elsif_stmt();
    } else if (this->is_token_in_firsts("else_block")) {
        if_node->next_else = this->else_block();
    }
    this->check_get_next(Type::endkw);
    this->check_get_next(Type::ifkw);
    return if_node;
};

ElifNode *Parser::elsif_stmt() {
    /*
    elsif_stmt:
        | ELSIFKW expression THENKW block elsif_stmt
        | ELSIFKW expression THENKW block else_block
        | ELSIFKW expression THENKW block
     */
    this->check_get_next(Type::elsifkw);
    ExpressionNode *condition = this->expression();
    this->check_get_next(Type::thenkw);
    BlockNode *body = this->block();
    ElifNode *elif_node = new ElifNode(condition, body);
    if (this->is_token_in_firsts("elsif_stmt")) {
        elif_node->next_elif = this->elsif_stmt();
    } else if (this->is_token_in_firsts("else_block")) {
        elif_node->next_else = this->else_block();
    }
    return elif_node;
};

ElseNode *Parser::else_block() {
    /*
    else_block:
        | ELSEKW block
     */
    this->check_get_next(Type::elsekw);
    BlockNode *body = this->block();
    return new ElseNode(body);
};

WhileNode *Parser::while_stmt() {
    /*
    while_stmt:
        | WHILEKW expression LOOPKW block ENDKW LOOPKW
     */
    this->check_get_next(Type::whilekw);
    ExpressionNode *condition = this->expression();
    this->check_get_next(Type::loopkw);
    BlockNode *body = this->block();
    this->check_get_next(Type::endkw);
    this->check_get_next(Type::loopkw);
    return new WhileNode(condition, body);
};

ForNode *Parser::for_stmt() {
    /*
    for_stmt:
        | FORKW ID IN (NUMBER | ID) DOUBLEDOT (NUMBER | ID) LOOPKW block ENDKW LOOPKW
     */
    this->check_get_next(Type::forkw);
    Leaf *iterator = new Leaf(this->check_get_next(Type::id));
    this->check_get_next(Type::in);
    Leaf* from;
    Leaf* to;
    if (this->token_matches_any({Type::number, Type::id})) {
        from = new Leaf(this->get_token());
        this->next_token();
    } else { this->error("for_stmt"); }
    this->check_get_next(Type::doubledot);
    if (this->token_matches_any({Type::number, Type::id})) {
        to = new Leaf(this->get_token());
        this->next_token();
    } else { this->error("for_stmt"); }
    this->check_get_next(Type::loopkw);
    BlockNode *body = this->block();
    this->check_get_next(Type::endkw);
    this->check_get_next(Type::loopkw);
    return new ForNode(iterator, from, to, body);
};

void Parser::simple_stmt(BlockNode *parent_block) {
    /*
    simple_stmt:
        | assignment
        | expression
        | return_stmt
     */
    if (this->is_token_in_firsts("assignment") && this->forward(1).getType() == Type::assign) {
        parent_block->add_child(this->assignment());
    } else if (this->is_token_in_firsts("expression")) {
        parent_block->add_child(this->expression());
    } else if (this->is_token_in_firsts("return_stmt")) {
        parent_block->add_child(this->return_stmt());
    } else {
        this->error("simple_stmt");
    }
};

AssignmentNode *Parser::assignment() {
    /*
    assignment:
        | ID ASSIGN expression
     */
    Leaf *left = new Leaf(this->check_get_next(Type::id));
    this->check_get_next(Type::assign);
    ExpressionNode *right = this->expression();
    return new AssignmentNode(left, right);
};

ReturnNode *Parser::return_stmt() {
    /*
    return_stmt:
        | RETURNKW expression
     */
    this->check_get_next(Type::returnkw);
    return new ReturnNode(this->expression());
};

ExpressionNode *Parser::expression() {
    /*
    expression:
        | disjunction
     */
    return this->disjunction();
};

ExpressionNode *Parser::disjunction() {
    /*
    disjunction:
        | conjunction OR disjunction
        | conjunction
     */
    ExpressionNode *left = this->conjunction();
    if (this->get_token().getType() == Type::orop) {
        Token op = this->check_get_next(Type::orop);
        ExpressionNode *right = this->disjunction();
        return new BinaryNode(left, new Leaf(op), right);
    } else {
        return left;
    }
};

ExpressionNode *Parser::conjunction() {
    /*
    conjunction:
        | inversion AND conjunction
        | inversion
     */
    ExpressionNode *left = this->inversion();
    if (this->get_token().getType() == Type::andop) {
        Token op = this->check_get_next(Type::andop);
        ExpressionNode *right = this->conjunction();
        return new BinaryNode(left, new Leaf(op), right);
    } else {
        return left;
    }
};

ExpressionNode *Parser::inversion() {
    /*
    inversion:
        | NOT inversion
        | comparison
     */
    if (this->get_token().getType() == Type::notop) {
        Token op = this->check_get_next(Type::notop);
        ExpressionNode *operand = this->inversion();
        return new UnaryNode(new Leaf(op), operand);
    } else {
        return this->comparison();
    }
};

ExpressionNode *Parser::comparison() {
    /*
    comparison:
        | sum GREATER comparison
        | sum LESS comparison
        | sum EQUAL comparison
        | sum NOTEQ comparison
        | sum GREQUAL comparison
        | sum LEQUAL comparison
        | sum
     */

    ExpressionNode *left = this->sum();
    if (this->token_matches_any({Type::greater, Type::less, Type::equal, Type::noteq, Type::grequal, Type::lequal})) {
        Token op = this->get_token();
        this->next_token();
        ExpressionNode *right = this->comparison();
        return new BinaryNode(left, new Leaf(op), right);
    } else {
        return left;
    }
}

ExpressionNode *Parser::sum() {
    /*
    sum:
        | term PLUS sum
        | term MINUS sum
        | term
     */
    ExpressionNode *left = this->term();
    if (this->token_matches_any({Type::plus, Type::minus})) {
        Token op = this->get_token();
        this->next_token();
        ExpressionNode *right = this->sum();
        return new BinaryNode(left, new Leaf(op), right);
    } else {
        return left;
    }
};

ExpressionNode *Parser::term() {
    /*
    term:
        | factor STAR term
        | factor DIV term
        | factor MOD term
        | factor
     */
    ExpressionNode *left = this->factor();
    if (this->token_matches_any({Type::star, Type::div, Type::mod})) {
        Token op = this->get_token();
        this->next_token();
        ExpressionNode *right = this->term();
        return new BinaryNode(left, new Leaf(op), right);
    } else {
        return left;
    }
};

ExpressionNode *Parser::factor() {
    /*
    factor:
        | PLUS factor
        | MINUS factor
        | primary
     */
    if (this->token_matches_any({Type::plus, Type::minus})) {
        Token op = this->get_token();
        this->next_token();
        ExpressionNode *operand = this->factor();
        return new UnaryNode(new Leaf(op), operand);
    } else {
        return this->primary();
    }
};

ExpressionNode *Parser::primary() {
    /*
    primary:
        | atom primary
        | atom
        | DOT ID primary
        | DOT ID
        | func_call
     */
    // Парсим цепочки типа ID.ID.ID.ID, пока не дойдём до вызова функции (например ID.func()), либо до конца цепочки
    VariableNode *variable = new VariableNode({});
    while (this->is_token_in_firsts("primary") && !this->is_token_in_firsts("func_call")) {
        if (this->is_token_in_firsts("atom")) {
            variable->add_to_chain(this->atom());
        } else {
            this->check_get_next(Type::dot);
            Token tk = this->check_get_next(Type::id);
            variable->add_to_chain(new Leaf(tk));
        }
    }
    if (this->is_token_in_firsts("func_call")) {
        ActualParamsNode *params = this->func_call();
        return new CallNode(variable, params);
    } else {
        return variable;
    }
};

ActualParamsNode *Parser::func_call() {
    /*
    func_call:
        | LPR actual_params RPR
        | LPR RPR
     */
    this->check_get_next(Type::lpr);
    if (this->is_token_in_firsts("actual_params")) {
        ActualParamsNode *params = this->actual_params();
        this->check_get_next(Type::rpr);
        return params;
    } else {
        this->check_get_next(Type::rpr);
        return new ActualParamsNode({});
    }
};

Leaf *Parser::atom() {
    /*
    atom:
        | ID
        | STRING
        | NUMBER
     */
    if (this->token_matches_any({Type::id, Type::string, Type::number})) {
        Leaf *leaf = new Leaf(this->get_token());
        this->next_token();
        return leaf;
    } else {
        this->error("atom");
        return nullptr;
    }
};

ActualParamsNode *Parser::actual_params() {
    /*
    arguments:
        | expression COMMA actual_params
        | expression
     */
    ActualParamsNode *params = new ActualParamsNode({});
    while (this->is_token_in_firsts("actual_params")) {
        params->add_child(this->expression());
        if (this->get_token().getType() == Type::comma) {
            this->next_token();
        }
    }
    return params;
};


