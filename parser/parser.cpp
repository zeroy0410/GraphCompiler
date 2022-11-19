#include "parser.h"
#include <stdlib.h>
#include <stdarg.h>

static ExprNode *MakeExprNode(enum token_type opcode, void *arg1, void *arg2);
static ExprNode *atom(void);
static ExprNode *component(void);
static ExprNode *factor(void);
static ExprNode *term(void);
static ExprNode *expression(void);

void FetchToken()
{
    cur_token = &GetToken();
    if (cur_token->type == ERRTOKEN)
        err_exit("error at fetching token.", nullptr, -EFAULT);
}

void MatchToken(enum Token_Type type)
{
    if (cur_token->type != type)
        err_exit("unexpected token: %s", (cur_token->lexeme ? cur_token->lexeme : "(null)"), -EFAULT);
    FetchToken();
}

ExprNode *MakeExprNode(Token_Type opcode, void *arg1, void *arg2)
{
    ExprNode *new_node;

    new_node = new ExprNode;
    new_node->OpCode = opcode;
    switch (opcode)
    {
    case CONST_ID:
        new_node->Content.CaseConst = *reinterpret_cast<double *>(arg1);
        break;
    case T:
        new_node->Content.CaseParmPtr = reinterpret_cast<double *>(arg1);
        break;
    case FUNC:
        new_node->Content.CaseFunc.MathFuncPtr = reinterpret_cast<double (*)(double)>(arg1);
        new_node->Content.CaseFunc.Child = reinterpret_cast<struct ExprNode *>(arg2);
        break;
    default:
        new_node->Content.CaseOperator.Left = reinterpret_cast<struct ExprNode *>(arg1);
        new_node->Content.CaseOperator.Right = reinterpret_cast<struct ExprNode *>(arg2);
        break;
    }

    return new_node;
}

ExprNode *atom()
{
    Token *per_token;
    ExprNode *new_node;
    ExprNode *per_expression;
    per_token = cur_token;
    switch (per_token->type)
    {
    case CONST_ID:
        MatchToken(CONST_ID);
        new_node = MakeExprNode(CONST_ID, &per_token->value, nullptr);
        break;
    case T:
        MatchToken(T);
        new_node = MakeExprNode(T, &(per_token->value), nullptr);
        break;
    case FUNC:
        MatchToken(FUNC);
        MatchToken(L_BRACKET);
        per_expression = expression();
        new_node = MakeExprNode(FUNC, reinterpret_cast<void *>(per_token->FuncPtr), per_expression);
        MatchToken(R_BRACKET);
        break;
    case L_BRACKET:
        MatchToken(L_BRACKET);
        new_node = expression();
        MatchToken(R_BRACKET);
        break;
    default:
        err_exit("Invalid expression", nullptr, -EFAULT);
        break;
    }
}

ExprNode *expression()
{
    ExprNode    *left;
    ExprNode    *right;
    Token_Type  type;

    left = term();
    while (cur_token->type == PLUS || cur_token->type == MINUS)
    {
        type = cur_token->type;
        MatchToken(type);
        right = term();
        left = MakeExprNode(type, 
                reinterpret_cast<void*>(left), 
                reinterpret_cast<void*>(right)
        );
    }
    return left;
}

ExprNode* term(void)
{
    ExprNode    *left;
    ExprNode    *right;
    Token_Type  type;

    left = factor();
    while (cur_token->type == MUL || cur_token->type == DIV)
    {
        type = cur_token->type;
        MatchToken(type);
        right = factor();
        left = MakeExprNode(type, 
                reinterpret_cast<void*>(left), 
                reinterpret_cast<void*>(right)
        );
    }
    return left;
}

ExprNode* factor(void)
{
    ExprNode    *left;
    ExprNode    *right;

    switch(cur_token->type)
    {
        case PLUS:
                MatchToken(PLUS);
                right = factor();
                left = new ExprNode;
                left->OpCode = PLUS;
                left->Content.CaseConst = 0.0;
                right = MakeExprNode(PLUS, left, right);
                break;
        case MINUS:
                MatchToken(MINUS);
                right = factor();
                left = new ExprNode;
                left->OpCode = CONST_ID;
                left->Content.CaseConst = 0.0;
                right = MakeExprNode(MINUS, left, right);
                break;
        default:
                right = component();
                break;
    }

    return right;
}

ExprNode* component(void)
{
    ExprNode    *left;
    ExprNode    *right;
    left = atom();
    if (cur_token->type == POWER)
    {
        MatchToken(POWER);
        right = component();
        left = MakeExprNode(POWER, left, right);
    }
    return left;
}

void origin_statement(void)
{
    ExprNode  *node;

    MatchToken(ORIGIN);
    MatchToken(IS);
    MatchToken(L_BRACKET);

    // get the value
    node = expression();
    // origin_x = get_expr_val(node);
    MatchToken(COMMA);
    node = expression();
    // origin_y = get_expr_val(node);
    MatchToken(R_BRACKET);
}

void scale_statement(void)
{
    ExprNode *node;

    MatchToken(SCALE);
    MatchToken(IS);
    MatchToken(L_BRACKET);

    node = expression();
    // scale_x = get_expr_val(node);

    MatchToken(COMMA);

    node = expression();
    // scale_y = get_expr_val(node);

    MatchToken(R_BRACKET);
}

void rot_statement(void)
{
    ExprNode    *node;

    MatchToken(ROT);
    MatchToken(IS);

    node = expression();
    // rot = get_expr_val(node);
}


void for_statement(void)
{
    ExprNode    *node;
    Token        *t;
    double       start;
    double       end;
    double       step;
    ExprNode    *node_x;
    ExprNode    *node_y;

    MatchToken(FOR);
    t = cur_token;
    MatchToken(T);

    MatchToken(FROM);
    node = expression();
    // start = get_expr_val(node);

    MatchToken(TO);
    node = expression();
    // end = get_expr_val(node);

    MatchToken(STEP);
    node = expression();
    // step = get_expr_val(node);

    MatchToken(DRAW);
    MatchToken(L_BRACKET);
    node_x = expression();
    MatchToken(COMMA);
    node_y = expression();
    MatchToken(R_BRACKET);

    // draw_loop(t, start, end, step, node_x, node_y);
}


void var_statement(void)
{
    ExprNode    *node;
    Token        *t;

    t = cur_token;
    MatchToken(T);
    MatchToken(IS);
    node = expression();
    // t->val = get_expr_val(node);
}

