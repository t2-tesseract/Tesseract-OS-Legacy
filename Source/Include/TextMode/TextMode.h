#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>

void TerminalClear();
void TerminalSetColor(uint8_t Color);
void TerminalPutChar(char c);
void TerminalWrite(const char* String);