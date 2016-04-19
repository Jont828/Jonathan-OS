#ifndef __SYSTEM_H
#define __SYSTEM_H

/* MAIN.C */
void *memcpy(void *dest, const void *src, int /*size_t*/ count);
void *memset(void *dest, char val, int /*size_t*/ count);
unsigned short *memsetw(unsigned short *dest, unsigned short val, int /*size_t*/ count);
int strlen(char *text);

#endif