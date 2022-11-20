#ifndef SCANNER_H
#define SCANNER_H

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

enum Token_Type
{
	ORIGIN /* 0 */,
	SCALE /* 1 */,
	ROT /* 2 */,
	IS /* 3 */,
	TO /* 4 */,
	STEP /* 5 */,
	DRAW /* 6 */,
	FOR /* 7 */,
	FROM /* 8 */, //保留字
	T /* 9 */,	  //参数
	SEMICO /* 10 */,
	L_BRACKET /* 11 */,
	R_BRACKET /* 12 */,
	COMMA /* 13 */,
	PLUS /* 14 */,
	MINUS /* 15 */,
	MUL /* 16 */,
	DIV /* 17 */,
	POWER /* 18 */,
	FUNC /*19 */,
	CONST_ID /* 20 */,
	NONTOKEN /* 21 */,
	ERRTOKEN /* 22 */
};

struct Token
{
	Token_Type type;		   //记号类别
	char *lexeme;			   //属性，字符串，指向char类型的指针
	double value;			   //属性，常数的值，double型
	double (*FuncPtr)(double); //属性，函数指针，代表一个指向返回double值并带有一个double形参的函数的指针的类型
};

static Token TokenTab[] = {
	{CONST_ID, "PI", 3.1415926, NULL},
	{CONST_ID, "E", 2.71828, NULL},
	{T, "T", 0.0, NULL},
	{FUNC, "SIN", 0.0, sin},
	{FUNC, "COS", 0.0, cos},
	{FUNC, "TAN", 0.0, tan},
	{FUNC, "LN", 0.0, log},
	{FUNC, "EXP", 0.0, exp},
	{FUNC, "SQRT", 0.0, sqrt},
	{ORIGIN, "ORIGIN", 0.0, NULL},
	{SCALE, "SCALE", 0.0, NULL},
	{ROT, "ROT", 0.0, NULL},
	{IS, "IS", 0.0, NULL},
	{FOR, "FOR", 0.0, NULL},
	{FROM, "FROM", 0.0, NULL},
	{TO, "TO", 0.0, NULL},
	{STEP, "STEP", 0.0, NULL},
	{DRAW, "DRAW", 0.0, NULL},
};

extern int InitScanner(const char *);
extern Token *GetToken();
extern void CloseScanner();

#endif