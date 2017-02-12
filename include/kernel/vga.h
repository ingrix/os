#ifndef __K_VGA_H__
#define __K_VGA_H__

#include <kernel.h>

//#define VGA_ADDR (uint16_t*)0xB8000
#define VGA_ADDR (uint16_t*)0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#define VGA_LINE_LEN 160 // bytes per line
#define VGA_MEM_SIZE VGA_WIDTH*VGA_HEIGHT*sizeof(uint16_t)

#define TERM_DEFAULT_FG 0x0F
#define TERM_DEFAULT_BG 0x00

#define VGA_FG(x) ((x) & 0x0F)
#define VGA_BG(x) ((x) & 0xF)

#define VGA_DEFAULT_COLOR vga_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK)

#define VGA_TERM_MODE_WRAP    0 // wraps text to top of screen when at bottom
#define VGA_TERM_MODE_SCROLL  1 // moves text up and frees a new line 

enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

void vga_term_init();
void vga_set_row(size_t r);
void vga_set_col(size_t c); 
size_t vga_get_row(); 
size_t vga_get_col(); 
uint8_t vga_color(uint8_t fg, uint8_t bg);
void vga_clear_screen(uint8_t color);
void vga_puts_color(const char *msg, uint8_t color);
void vga_puts(const char *msg);
void vga_putchar_color(char c, uint8_t color);
void vga_putchar(char c);

#endif // __K_VGA_H__
