#include "scanner.h"
int main()
{
    InitScanner("text.txt");
    while (1)
    {
        Token token = GetToken();
        if (token.type != NONTOKEN)
            printf("%4d,%12s,%12f,%12x\n", token.type, token.lexeme, token.value, token.FuncPtr);
        else
            break;
    }
    CloseScanner();
    return 0;
}