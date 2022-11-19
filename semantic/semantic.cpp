#include "errorReporter/errorReporter.h"
#include "scanner/scanner.h"
#include "parser/parser.h"
#include "semantic.h"
#include <iostream>
#include <cmath>

using namespace std;

double scale_x;
double scale_y;
double origin_x;
double origin_y;
double rot;

double get_expr_val(ExprNode *tree)
{
    double Right_val;

    switch (tree->OpCode)
    {
        case PLUS:
                return get_expr_val(tree->Content.CaseOperator.Left) + get_expr_val(tree->Content.CaseOperator.Right);
        case MINUS:
                return get_expr_val(tree->Content.CaseOperator.Left) - get_expr_val(tree->Content.CaseOperator.Right);
        case MUL:
                return get_expr_val(tree->Content.CaseOperator.Left) * get_expr_val(tree->Content.CaseOperator.Right);
        case DIV:
                return get_expr_val(tree->Content.CaseOperator.Left) / get_expr_val(tree->Content.CaseOperator.Right);
        case POWER:
                return pow(get_expr_val(tree->Content.CaseOperator.Left), get_expr_val(tree->Content.CaseOperator.Right));
        case FUNC:
                return (tree->Content.CaseFunc.MathFuncPtr)(get_expr_val(tree->Content.CaseFunc.Child));
        case T:
                return *(tree->Content.CaseParmPtr);
        case CONST_ID:
                return tree->Content.CaseConst;
        default:
                return 0.0;
    }
}