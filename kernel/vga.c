#include <vga.h>

static size_t term_row;
static size_t term_col;
static size_t term_color;

// vga entry, high byte = color, low byte = character
static inline uint16_t vga_entry(char c, uint8_t color) {
  return (uint16_t)c | (uint16_t)color << 8;
}

// color: upper 4 bits: background, lower 4: foreground
inline uint8_t vga_color(uint8_t fg, uint8_t bg) {
  return fg | bg << 4;
}

void vga_set_row(size_t r) {
  if(r >= VGA_HEIGHT) {
    r = r % VGA_HEIGHT;
  }
  term_row = r;
}

void vga_set_col(size_t c) {
  if(c >= VGA_WIDTH) {
    c = c % VGA_WIDTH;
  }
  term_col = c;
}

size_t vga_get_row() {
  return term_row;
}

size_t vga_get_col() {
  return term_col;
}

void vga_next_line() {
  if(++term_row == VGA_HEIGHT) {
    term_row = 0;
  }
  term_col = 0;
}

void vga_next_char() {
  if(++term_col == VGA_WIDTH) {
    term_col = 0;
    if(++term_row == VGA_HEIGHT) {
      term_row = 0;
    }
  }
}

void vga_print_char(char c, uint8_t color) {
  uint16_t *vga_buff = (uint16_t*)VGA_ADDR;

  switch(c) {
    case '\n' :
      vga_next_line();
      return;
    default :
      vga_buff[term_row * VGA_WIDTH + term_col] = vga_entry(c, color);
  }

  vga_next_char();

}
void vga_print_msg(const char *msg, uint8_t color) {
  if(!msg) {
    return;
  }
  for( ; *msg; msg++) {
    vga_print_char(*msg, color);
  }
}

void vga_clear_screen(uint8_t color) {
  vga_set_row(0);
  vga_set_col(0);
  for(int i=0; i < VGA_WIDTH * VGA_HEIGHT; ++i) {
    vga_print_char(' ', color);
  }
}

void vga_term_init() {
  term_row = 0;
  term_col = 0;
  term_color = vga_color(TERM_DEFAULT_FG, TERM_DEFAULT_BG);
  vga_clear_screen(term_color);
  term_row = 0;
  term_col = 0;
}

