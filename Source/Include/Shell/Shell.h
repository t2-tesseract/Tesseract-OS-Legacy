#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>

struct UserVar {
    char *Name;
    void *Value;
    char *Next;
};

extern struct UserVar *userVars;

void ExecuteCommand(char *Input, char *Arg1, char *Arg2);