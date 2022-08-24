#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <Include/Common/Common.h>

#define VgaCtrlRegister 0x3d4
#define VgaDataRegister 0x3d5
#define VgaOffsetLow 0x0f
#define VgaOffsetHigh 0x0e

void* memmove(void* dstptr, const void* srcptr, size_t size);
void TerminalClear(bool resetPos);
void TerminalSetColor(uint8_t Color);
void TerminalBack();
void SetCharAtVideoMemory(char Character, int Offset);
void TerminalPutChar(char c);
void TerminalWrite(const char* String);
void TerminalShell();
int GetCursor();
int CompareString(char String1[], char String2[]);
void ExecuteCommand(char *Input);