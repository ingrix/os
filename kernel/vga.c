#include <string.h>
#include <vga.h>
#include <io.h>

static size_t term_row;
static size_t term_col;
static size_t term_color;
static int term_mode;

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
    if(term_mode == VGA_TERM_MODE_WRAP) {
      term_row = 0;
    } else if(term_mode == VGA_TERM_MODE_SCROLL) {
      uint16_t *vga_buff = VGA_ADDR;
      // move all characters up a row
      memmove((void*)VGA_ADDR, VGA_ADDR + VGA_WIDTH, VGA_MEM_SIZE - VGA_LINE_LEN);
      // clear the last row
      --term_row;
      memset(&vga_buff[term_row * VGA_WIDTH], 0, VGA_LINE_LEN);
    }
  }
  term_col = 0;
}

void vga_next_char() {
  if(++term_col == VGA_WIDTH) {
    vga_next_line();
  }
}

void vga_prev_char() {
  if(term_col-- == 0) {
    term_col = VGA_WIDTH - 1;
    if(term_row-- == 0) {
      term_row = VGA_HEIGHT - 1;
    }
  }
}

void vga_cursor_disable() {
  // address and data crt registers
  uint16_t vga_crtc_a = 0x3B4, vga_crtc_d;
  uint8_t old_a;
  
  // get the misc register data 
  uint8_t mr = ior_byte(0x3CC);
  if(mr & 1) {
    vga_crtc_a = 0x3D4;
  }
  vga_crtc_d = vga_crtc_a + 1;

  old_a = ior_byte(vga_crtc_a);

  // cursor start register
  iow_byte(vga_crtc_a, 0x0a);
  uint8_t csr = ior_byte(vga_crtc_d);
  csr |= (1 << 5); // set cursor disable bit
  iow_byte(vga_crtc_d, csr);
  iow_byte(vga_crtc_a, old_a);
}

void vga_cursor_enable() {
  // address and data crt registers
  uint16_t vga_crtc_a = 0x3B4, vga_crtc_d;
  uint8_t old_a;
  
  // get the misc register data 
  uint8_t mr = ior_byte(0x3CC);
  if(mr & 1) {
    vga_crtc_a = 0x3D4;
  }
  vga_crtc_d = vga_crtc_a + 1;

  old_a = ior_byte(vga_crtc_a);

  // cursor start register
  iow_byte(vga_crtc_a, 0x0a);
  uint8_t csr = ior_byte(vga_crtc_d);
  csr &= ~(1 << 5); // clear cursor disable bit
  iow_byte(vga_crtc_d, csr);
  iow_byte(vga_crtc_a, old_a);
}

void vga_putchar_color(char c, uint8_t color) {
  uint16_t *vga_buff = (uint16_t*)VGA_ADDR;

  switch(c) {
    case '\n' :
      vga_next_line();
      return;
    case 0x08 : // backspace
      vga_prev_char();
      vga_buff[term_row * VGA_WIDTH + term_col] = vga_entry(' ', color);
      return;
    default :
      if(c < 127 && c > 31) {
        vga_buff[term_row * VGA_WIDTH + term_col] = vga_entry(c, color);
      } else {
        vga_buff[term_row * VGA_WIDTH + term_col] = vga_entry('.', color);
      }
  }

  vga_next_char();
}

void vga_putchar(char c) {
  vga_putchar_color(c, VGA_DEFAULT_COLOR);
}

void vga_puts_color(const char *msg, uint8_t color) {
  if(!msg) {
    return;
  }
  for( ; *msg; ++msg) {
    vga_putchar_color(*msg, color);
  }
}

void vga_puts(const char *msg) {
  vga_puts_color(msg, VGA_DEFAULT_COLOR);
}

void vga_clear_screen(uint8_t color) {
  vga_set_row(0);
  vga_set_col(0);
  for(int i=0; i < VGA_WIDTH * VGA_HEIGHT; ++i) {
    vga_putchar_color(' ', color);
  }
}

void vga_term_init() {
  term_row = 0;
  term_col = 0;
  term_color = vga_color(TERM_DEFAULT_FG, TERM_DEFAULT_BG);
  vga_clear_screen(term_color);
  term_row = 0;
  term_col = 0;
  term_mode = VGA_TERM_MODE_SCROLL;
  vga_cursor_disable();
}

