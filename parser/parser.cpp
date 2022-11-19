#include"parser.h"
#include<stdlib.h>
#include<stdarg.h>

void FetchToken()
{
    cur_token = &GetToken();
    if (cur_token->type == ERRTOKEN)
        err_exit("error at fetching token.", nullptr, -EFAULT);
}

void MatchToken(enum Token_Type type)
{
    if(cur_token->type!=type)
        err_exit("unexpected token: %s",(cur_token->lexeme ? cur_token->lexeme : "(null)"), -EFAULT);
    FetchToken();  
}

ExprNode* MakeExprNode(Token_Type opcode,void *arg1,void *arg2){
    ExprNode *new_node;

    new_node = new ExprNode;
    new_node->OpCode = opcode;
    switch (opcode)
    {
        case CONST_ID:
                new_node->Content.CaseConst = *reinterpret_cast<double*>(arg1);
                break;
        case T:
                new_node->Content.CaseParmPtr = reinterpret_cast<double*>(arg1);
                break;
        case FUNC:
                new_node->Content.CaseFunc.MathFuncPtr = reinterpret_cast<double (*)(double)>(arg1);
                new_node->Content.CaseFunc.Child = reinterpret_cast<struct ExprNode*>(arg2);
                break;
        default:
                new_node->Content.CaseOperator.Left = reinterpret_cast<struct ExprNode*>(arg1);
                new_node->Content.CaseOperator.Right = reinterpret_cast<struct ExprNode*>(arg2);
                break; 
    }

    return new_node;
}

