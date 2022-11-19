#include <stdio.h>
#include <stdlib.h>

void err_exit(const char *msg, const char *err, int err_code)
{
    printf("Compiler fatal error: ");
    printf(msg, err);
    puts("\ncompilation terminated.");
    exit(err_code);
}