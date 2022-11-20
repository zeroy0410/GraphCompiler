#include "semantic.h"
#include <iostream>
#include <cmath>

using namespace std;

double scale_x;
double scale_y;
double origin_x;
double origin_y;
double rot;

bool is_init_drawer = false;
FILE *draw_py_file;

void InitDrawer()
{
    draw_py_file = fopen("./drawer.py", "w");
    if (!draw_py_file)
        err_exit("unable to create py file, draw failed.", nullptr, -EFAULT);
    is_init_drawer = true;

    fprintf(draw_py_file, "from tkinter import *\n");
    fprintf(draw_py_file, "tk = Tk()\n");
    fprintf(draw_py_file, "canvas = Canvas(tk, width = 1200, height = 800)\n");
    fprintf(draw_py_file, "canvas.pack()\n");
}

void CloseDrawer()
{
    if (draw_py_file)
    {
        fprintf(draw_py_file, "input()\n");
        fclose(draw_py_file);
    }
}

void DrawPixel(double x, double y)
{
    if (!is_init_drawer)
        InitDrawer();

    if (fabs(x) != NAN && fabs(x) != INFINITY && fabs(y) != NAN && fabs(y) != INFINITY)
        fprintf(draw_py_file, "canvas.create_oval(%lf, %lf, %lf, %lf)\n", x, y, x, y);
}

void GetCoordinate(ExprNode *exp_x, ExprNode *exp_y, double &x, double &y)
{
    double coordinate_x;
    double coordinate_y;
    double tmp;

    coordinate_x = GetExprVal(exp_x);
    coordinate_y = GetExprVal(exp_y);
    // cout << coordinate_x << ' ' << coordinate_y << endl;
    coordinate_x *= scale_x;
    coordinate_y *= scale_y;
    tmp = coordinate_x * cos(rot) + coordinate_y * sin(rot);
    coordinate_y = coordinate_y * cos(rot) - coordinate_x * sin(rot);
    coordinate_x = tmp;
    coordinate_x += origin_x;
    coordinate_y += origin_y;

    x = coordinate_x;
    y = coordinate_y;
}

double GetExprVal(ExprNode *tree)
{
    double Right_val;

    switch (tree->OpCode)
    {
    case PLUS:
        return GetExprVal(tree->Content.CaseOperator.Left) + GetExprVal(tree->Content.CaseOperator.Right);
    case MINUS:
        return GetExprVal(tree->Content.CaseOperator.Left) - GetExprVal(tree->Content.CaseOperator.Right);
    case MUL:
        return GetExprVal(tree->Content.CaseOperator.Left) * GetExprVal(tree->Content.CaseOperator.Right);
    case DIV:
        return GetExprVal(tree->Content.CaseOperator.Left) / GetExprVal(tree->Content.CaseOperator.Right);
    case POWER:
        return pow(GetExprVal(tree->Content.CaseOperator.Left), GetExprVal(tree->Content.CaseOperator.Right));
    case FUNC:
        return (tree->Content.CaseFunc.MathFuncPtr)(GetExprVal(tree->Content.CaseFunc.Child));
    case T:
        // cout<<"haha2: "<<tree->Content.CaseParmPtr<<endl;
        return *(tree->Content.CaseParmPtr);
    case CONST_ID:
        return tree->Content.CaseConst;
    default:
        return 0.0;
    }
}

void DrawLoop(Token *param, double start, double end, double step, ExprNode *exp_x, ExprNode *exp_y)
{

    double x;
    double y;
    if (!is_init_drawer)
        InitDrawer();
    // cout << scale_x << ' ' << scale_y << ' ' << rot << ' '<<origin_x << ' ' << origin_y << endl;
    // cout<<"haha1: "<<&(param->value)<<endl;
    for (param->value = start; param->value <= end; param->value += step)
    {
        GetCoordinate(exp_x, exp_y, x, y);
        DrawPixel(x, y);
        // printf("X Y STEP %f %f %f\n",x,y,step);
    }
}