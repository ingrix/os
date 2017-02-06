#ifndef __K_STRING_H__
#define __K_STRING_H__

#include <kernel/kernel.h>

#define memcpy __builtin_memcpy
#define memset __builtin_memset
#define memcmp __builtin_memcmp

#define strcpy __builtin_strcpy
#define strcmp __builtin_strcmp
#define strlen __builtin_strlen
#define strncmp __builtin_strncmp
#define strncpy __builtin_strncpy
#define strnlen __builtin_strnlen
/*
void *memset(void *dest, int c, size_t n);
void *memset_simple(void *dest, int c, size_t n);

void *memcpy(void *dest, const void *src, size_t n);
void *memcpy_simple(void *dest, const void *src, size_t n);

*/
void *memmove(void *dest, void *src, size_t n);
/*
void *memmove_simple(void *dest, void *src, size_t n);

int memcmp(const void *s1, const void *s2, size_t n);

size_t strlen(const char *buf);
size_t strnlen(const char *buf, size_t maxlen);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
*/

#endif // __K_STRING_H__
