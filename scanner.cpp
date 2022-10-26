#include "scanner.h"
#define MAX_TOKEN_LEN 105
static FILE *InFile;
static char TokenBuffer[MAX_TOKEN_LEN];

void SetTokenBufferEmpty(){
    memset(TokenBuffer,0,MAX_TOKEN_LEN);
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