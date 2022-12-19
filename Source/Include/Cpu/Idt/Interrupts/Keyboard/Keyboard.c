#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <Include/Common/Common.h>

static char KeyBuffer[256];

#define BackSpace 0x0E
#define Enter 0x1C

const char KeyboardMap[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',
  	'9', '0', '-', '=', '\b',
  	'\t',
  	'q', 'w', 'e', 'r',
  	't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
  	  0,
  	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ':',
 '	\'', '`',   0,
 '	\\', 'z', 'x', 'c', 'v', 'b', 'n',
  	'm', ',', '.', '/',   0,
  	'*',
  	  0,
  	' ',
  	  0,
  	  0,
  	  0,   0,   0,   0,   0,   0,   0,   0,
  	  0,
  	  0,
  	  0,
  	  0,
  	  0,
  	  0,
  	'-',
  	  0,
  	  0,
  	  0,
  	'+',
  	  0,
  	  0,
  	  0,
  	  0,
  	  0,
  	  0,   0,   0,
  	  0,
  	  0,
  	  0,
};

void KeyboardHandler(struct Registers *r){
    uint8_t Scancode = Inb(0x60);

    if (Scancode > 57) return;

	if (Scancode == BackSpace) {
        if (Backspace(KeyBuffer) == true ) {
            TerminalBack();
		}
	} else if (Scancode == Enter) {
		TerminalWrite("\n");

		/*char *KeyBufferArg = StringTok(KeyBuffer, ":");
		KeyBufferArg = StringTok(NULL, ":");

        ExecuteCommand(KeyBuffer, KeyBufferArg);*/

		char *token;
		char var1[999], var2[999], var3[999];

		token = StringTok(KeyBuffer, ":");
		StringCopy(var1, token);

		// get the second token
		token = StringTok(NULL, ":");
		StringCopy(var2, token);

		// get the third token
		token = StringTok(NULL, ":");
		StringCopy(var3, token);

		ExecuteCommand(var1, var2, var3);
        KeyBuffer[0] = '\0';
    } else {
		char Letter = KeyboardMap[(int) Scancode];
		Append(KeyBuffer, Letter);
		char String[2] = {Letter, '\0'};
		TerminalWrite(String);
	}
}

void KeyboardInstall(){
	IrqInstallHandler(1, KeyboardHandler);
}