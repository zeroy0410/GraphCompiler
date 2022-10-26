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

bool InitScanner(const char *FileName)
{
    InFile = fopen(FileName, "r");
    if (InFile != NULL)
        return true;
    return false;
}

void CloseScanner()
{
    if (InFile != NULL)
        fclose(InFile);
}

char GetChar()
{ // change all character to Capital letter
    char c = getc(InFile);
    // cout<<c<<endl;
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
    memset(&errortoken, 0, sizeof(Token)); //局部变量需置零
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

Token GetToken()
{
    Token token;
    memset(&token, 0, sizeof(Token));
    char c;
    SetTokenBufferEmpty();
    token.lexeme = TokenBuffer;
    while (1)
    {
        c = GetChar();
        if (c == EOF)
        {
            token.type = NONTOKEN;
            return token;
        }
        if (c != ' ')
            break;
    }
    AddToTokenBuffer(c);
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
        token = CheckKeyToken(TokenBuffer);
        token.lexeme = TokenBuffer;
        // cout<<token.type<<' '<<token.lexeme<<endl;
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
            BackChar(c);
            token.type = CONST_ID;
            token.value = atof(TokenBuffer);
            return token;
        }
    }
    else
    {
        switch (c)
        {
        case ';':
            token.type = SEMICO;
            break;
        case '(':
            token.type = L_BRACKET;
            break;
        case ')':
            token.type = R_BRACKET;
            break;
        case ',':
            token.type = COMMA;
            break;
        case '+':
            token.type = PLUS;
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
                token.type = MINUS;
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
                token.type = DIV;
                break;
            }
        case '*':
            c = GetChar();
            if (c == '*')
            {
                token.type = POWER;
                break;
            }
            else
            {
                BackChar(c);
                token.type = MUL;
                break;
            }
        default:
            token.type = ERRTOKEN;
            break;
        }
    }
}
