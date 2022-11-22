#include "scanner.h"
#include <iostream>
using namespace std;
#define MAX_TOKEN_LEN 105
static FILE *InFile;
static char TokenBuffer[MAX_TOKEN_LEN];

void SetTokenBufferEmpty()
{
    memset(TokenBuffer, 0, MAX_TOKEN_LEN);
}

extern int InitScanner(const char *FileName)
{
    InFile = fopen(FileName, "r");
    if (InFile != NULL)
        return true;
    return false;
}

extern void CloseScanner()
{
    if (InFile != NULL)
        fclose(InFile);
}

char GetChar()
{
    char c = getc(InFile);
    return toupper(c);
}

void BackChar(const char c)
{
    if (c != EOF)
        ungetc(c, InFile);
}

Token CheckKeyToken(const char *NowString)
{
    int TokenTabLen = sizeof(TokenTab) / sizeof(TokenTab[0]);
    for (int i = 0; i < TokenTabLen; i++)
    {
        if (strcmp(TokenTab[i].lexeme, NowString) == 0)
            return TokenTab[i];
    }
    Token errortoken;
    memset(&errortoken, 0, sizeof(Token));
    errortoken.type = ERRTOKEN;
    return errortoken;
}

void AddToTokenBuffer(char c)
{
    int len = strlen(TokenBuffer);
    TokenBuffer[len] = c;
    len++;
    TokenBuffer[len + 1] = '\0';
}

extern Token *GetToken()
{
    Token *token = new Token;
    char c;
    SetTokenBufferEmpty();
    token->lexeme = TokenBuffer;
    while (1)
    {
        c = GetChar();
        if (c == EOF)
        {
            token->type = NONTOKEN;
            return token;
        }
        if (c != ' ' && c != 13 && c != 10)
            break;
    }
    AddToTokenBuffer(c);
    // printf("C: %d %c\n",c,c);
    if (isalpha(c))
    {
        while (1)
        {
            c = GetChar();
            if (isalnum(c))
            {
                AddToTokenBuffer(c);
            }
            else
                break;
        }
        BackChar(c);
        // printf("CC: %d %c\n",c,c);
        token->type = CheckKeyToken(TokenBuffer).type;
        token->FuncPtr = CheckKeyToken(TokenBuffer).FuncPtr;
        token->value = CheckKeyToken(TokenBuffer).value;
        token->lexeme = TokenBuffer;
        if (token->type == 9)
            token = &TokenTab[2];
        return token;
    }
    else if (isdigit(c))
    {
        while (1)
        {
            c = GetChar();
            if (isdigit(c))
                AddToTokenBuffer(c);
            else
                break;
        }
        if (c == '.')
        {
            AddToTokenBuffer(c);
            while (1)
            {
                c = GetChar();
                if (isdigit(c))
                    AddToTokenBuffer(c);
                else
                    break;
            }
        }
        BackChar(c);
        token->type = CONST_ID;
        token->value = atof(TokenBuffer);
        return token;
    }
    else
    {
        switch (c)
        {
        case ';':
            token->type = SEMICO;
            break;
        case '(':
            token->type = L_BRACKET;
            break;
        case ')':
            token->type = R_BRACKET;
            break;
        case ',':
            token->type = COMMA;
            break;
        case '+':
            token->type = PLUS;
            break;
        case '-':
            c = GetChar();
            if (c == '-')
            {
                while (c != '\n' && c != EOF)
                    c = GetChar();
                BackChar(c);
                return GetToken();
            }
            else
            {
                BackChar(c);
                token->type = MINUS;
                break;
            }
        case '/':
            c = GetChar();
            if (c == '/')
            {
                while (c != '\n' && c != EOF)
                    c = GetChar();
                BackChar(c);
                return GetToken();
            }
            else
            {
                BackChar(c);
                token->type = DIV;
                break;
            }
        case '*':
            c = GetChar();
            if (c == '*')
            {
                token->type = POWER;
                break;
            }
            else
            {
                BackChar(c);
                token->type = MUL;
                break;
            }
        default:
            token->type = ERRTOKEN;
            break;
        }
    }
    return token;
}
