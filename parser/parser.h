#ifndef PARSER_H
#define PARSER_H

#include "../scanner/scanner.h"
#include "../errorReporter/errorReporter.h"

typedef double (*FuncPtr)(double);
struct ExprNode
{
    Token_Type OpCode;
    union
    {
        struct
        {
            ExprNode *Left, *Right;
        } CaseOperator;
        struct
        {
            ExprNode *Child;
            FuncPtr MathFuncPtr;
        } CaseFunc;
        double CaseConst;
        double *CaseParmPtr;
    } Content;
};

static struct Token *cur_token;

extern void parser(void);

#endif