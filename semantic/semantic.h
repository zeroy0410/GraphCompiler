#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "../parser/parser.h"
#include "../errorReporter/errorReporter.h"
extern double scale_x;
extern double scale_y;
extern double origin_x;
extern double origin_y;
extern double rot;

extern double GetExprVal(ExprNode *tree);
extern void DrawLoop(Token *param, double start, double end, double step, ExprNode *exp_x, ExprNode *exp_y);
extern void CloseDrawer();

#endif