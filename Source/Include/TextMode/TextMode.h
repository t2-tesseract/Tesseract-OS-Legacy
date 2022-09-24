#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <Kernel/Log.h>
#include <Include/Common/Common.h>

#define VgaCtrlRegister 0x3d4
#define VgaDataRegister 0x3d5
#define VgaOffsetLow 0x0f
#define VgaOffsetHigh 0x0e

extern struct File *Files;

int GetRowFromOffset(int Offset);
int GetOffset(int Col, int Row);
int MoveOffsetToNewLine(int Offset);
int Scroll(int Offset);
void TerminalClear();
void TerminalSetColor(uint8_t Color);
void TerminalBack();
void TerminalPutChar(char Character, int Offset);
void TerminalWrite(const char* String);
void DebugWrite(const char* String, int Mode, bool toEmuConsole);
void TerminalShell();
void SetCursor(int Offset);
int GetCursor();
int CompareString(char String1[], char String2[]);
void ExecuteCommand(char *Input);