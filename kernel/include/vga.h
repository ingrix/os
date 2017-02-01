#ifndef __K_VGA_H__
#define __K_VGA_H__

#include <stdint.h>
#include <stddef.h>

#define VGA_ADDR 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define TERM_DEFAULT_FG 0x0F
#define TERM_DEFAULT_BG 0x00

#define VGA_FG(x) ((x) & 0x0F)
#define VGA_BG(x) ((x) & 0xF)

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
void vga_print_msg(const char *msg, uint8_t color);
void vga_print_char(char c, uint8_t color);

#endif // __K_VGA_H__