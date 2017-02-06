#include <kernel/kernel.h>
#include <stdarg.h>
#include <kernel/printk.h>
#include <kernel/vga.h>

void putchar(int c) {
  vga_putchar(c);
}

void puts(const char *s) {
  for( ; *s; ++s) {
    putchar(*s);
  }
}

void printk(const char *fmt, ...) {
  va_list va;
  va_start(va, fmt);
  char buf[20];

  const char *c = fmt;
  while(*c) {
    if(*c != '%') {
      putchar(*c++);
      continue;
    }

    char *p;
    ++c;
    switch(*c) {
      case 'x' :
      case 'd' :
      case 'u' :
      case 'p' :
        itoa(buf, *c, va_arg(va, int));
        p = buf;
        ++c;
        goto string;
        break;
      case 's' :
        p = va_arg(va, char *);
        ++c;
      string :
        while(*p) {
          putchar(*p++);
        }
        break;
    }
  }
  va_end(va);
}

void itoa(char *buf, int base, int d) {
  unsigned long ud = d;
  unsigned short n = 0;
  int divisor = 16;
  char *p = buf, tmp;
  char *p1, *p2;

  switch(base) {
    case 'd' :
      divisor = 10;
      if(d < 0) {
        *p++ = '-';
        buf++;
        ud = -d;
      }
    default:
      break;
  }

  do {
    int remainder = ud % divisor;
    *p++ = (remainder < 10) ? remainder + '0' : remainder + '0' - 10;
    ++n;
  } while(ud /= divisor);

  if(base == 'p') {
    while(n < (sizeof(void*) * 2)) {
      *p++ = '0';
      ++n;
    }
  }

  // add hex prefix
  if(divisor == 16) {
    *p++ = 'x';
    *p++ = '0';
  }
  *p = 0;

  // characters are small->big so reverse buffer
  p1 = buf;
  p2 = p - 1;
  while(p1 < p2) {
    tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
    ++p1, --p2;
  }
}
