#include <string.h>

// memset which sets memory in 4-byte chunks
void *memset(void *dest, int c, size_t n) {
  uint32_t c8 = (uint32_t)c;
  uint32_t fill = (c8 << 24) | (c8 << 16) | (c8 << 8) | c;

  size_t nchunks = n >> 2; // number of 4-byte chunks
  size_t nspare = n & 0x03; // number of leftover bytes

  // how many spare bytes there are
  uint32_t *dptr32 = (uint32_t*)dest;
  for(size_t i = 0; i < nchunks; ++i) {
    *dptr32++ = fill;
  }

  uint8_t *dptr8 = (uint8_t*)dptr32; 
  for(size_t i = 0; i < nspare; ++i) {
    *dptr8++ = fill;
  }
  return dest;
}

// single-byte-op reference memset
void *memset_simple(void *dest, int c, size_t n) {
  for(size_t i=0; i < n; ++i) {
    *(((uint8_t*)dest)+i) = (uint8_t)c;
  }
  return dest;
}

// memcpy which copies in 32-byte chunks
void *memcpy(void *dest, const void *src, size_t n) {
  uint32_t *dptr32 = (uint32_t*)dest;
  uint32_t *sptr32 = (uint32_t*)src;

  size_t nchunks = n >> 2; // number of 4-byte chunks
  size_t nspare = n & 0x03; // number of leftover bytes

  for(size_t i = 0; i < nchunks; ++i) {
    *dptr32++ = *sptr32++;
  }

  uint8_t *dptr8 = (uint8_t*)dptr32; 
  uint8_t *sptr8 = (uint8_t*)sptr32; 
  for(size_t i = 0; i < nspare; ++i) {
    *dptr8++ = *sptr8++;
  }
  return dest;
}

// single-byte-operation reference memcpy
void *memcpy_simple(void *dest, const void *src, size_t n) {
  for(size_t i=0; i < n; ++i) {
    *((uint8_t*)dest++) = *((uint8_t*)src++);
  } return dest; 
}

// memmove which reads in 32-byte chunks
void *memmove(void *dest, void *src, size_t n) {

  size_t nchunks = n >> 2; // number of 4-byte chunks
  size_t nspare = n & 0x03; // number of leftover bytes

  int inc = 1;
  if(dest > src && dest < src + n) { // overlapping memory, start from back of buffer
    dest += n;
    src += n;
    inc = -1;
    if(nchunks) {
      dest -= sizeof(uint32_t);
      src -= sizeof(uint32_t);
    } else {
      --dest;
      --src;
    }
  }

  uint32_t *dptr32 = dest;
  uint32_t *sptr32 = src;

  for(size_t i = 0; i < nchunks; ++i) {
    *dptr32 = *sptr32;
    dptr32 += inc;
    sptr32 += inc;
  }

  uint8_t *dptr8 = (uint8_t*)dptr32; 
  uint8_t *sptr8 = (uint8_t*)sptr32; 

  // if inc < 0 then we overshot by 3 bytes
  if(inc < 0) {
    dptr8 += sizeof(uint32_t)-1;
    sptr8 += sizeof(uint32_t)-1;
  }

  for(size_t i = 0; i < nspare; ++i) {
    *dptr8 = *sptr8;
    dptr8 += inc;
    sptr8 += inc;
  }
  return dest;
}

// single-byte-operation memmove
void *memmove_simple(void *dest, void *src, size_t n) {
  uint8_t *dptr = dest, *sptr = src, inc = 1;
  if(dest > src && dest < src + n) { // overlapping memory, start from back of buffer
    dest += n - 1;
    src += n - 1;
    inc = -1;
  }
  for(size_t i=0; i < n; ++i) {
    *dptr = *sptr;
    dptr += inc;
    sptr += inc;
  }
  return dest;
}

int memcmp(const void *s1, const void *s2, size_t n) {
  int r = 0;
  for(size_t i = 0; i < n && r == 0; ++i) {
    r = *(int8_t *)s1 - *(int8_t *)s2;
  }
  return r;
}

size_t strlen(const char *buf) {
  size_t i=0;
  while(*buf++) {
    ++i;
  }
  return i;
}

size_t strnlen(const char *buf, size_t maxlen) {
  size_t i=0;
  while(i < maxlen && *buf++) {
    ++i;
  }
  return i;
}

int strncmp(const char *s1, const char *s2, size_t n) {
  return memcmp(s1, s2, n);
}

int strcmp(const char *s1, const char *s2) {
  int r = 0;
  for(size_t i = 0; s1[i] && s2[i] && r == 0; ++i) {
    r = s1[i] - s2[i];
  }
  return r;
}
