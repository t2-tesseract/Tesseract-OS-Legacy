#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "Graphics.h"

struct VbeModeInfoStructure VbeMode;

void PutPixel(int X, int Y, unsigned int Color){
    unsigned char *Screen = VbeMode.framebuffer;
	unsigned int Where = X * 4 + Y * 1920 * 4;
	
	Screen[Where + 2] = (Color >> 16) & 255;
	Screen[Where + 1] = (Color >> 8) & 255;
	Screen[Where] = Color & 255;
}