#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct VbeModeInfoStructure {
	unsigned short attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	unsigned char window_a;			// deprecated
	unsigned char window_b;			// deprecated
	unsigned short granularity;		// deprecated; used while calculating bank numbers
	unsigned short window_size;
	unsigned short segment_a;
	unsigned short segment_b;
	unsigned int win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	unsigned short pitch;			// number of bytes per horizontal line
	unsigned short width;			// width in pixels
	unsigned short height;			// height in pixels
	unsigned char w_char;			// unused...
	unsigned char y_char;			// ...
	unsigned char planes;
	unsigned char bpp;			// bits per pixel in this mode
	unsigned char banks;			// deprecated; total number of banks in this mode
	unsigned char memory_model;
	unsigned char bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	unsigned char image_pages;
	unsigned char reserved0;
 
	unsigned char red_mask;
	unsigned char red_position;
	unsigned char green_mask;
	unsigned char green_position;
	unsigned char blue_mask;
	unsigned char blue_position;
	unsigned char reserved_mask;
	unsigned char reserved_position;
	unsigned char direct_color_attributes;
 
	unsigned int framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
	unsigned int off_screen_mem_off;
	unsigned short off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	unsigned char reserved1[206];
} __attribute__ ((packed));

void PutPixel(int X, int Y, unsigned int Color);