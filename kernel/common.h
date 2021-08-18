#ifndef __COMMON_H__
#define __COMMON_H__

#include "types.h"

void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
void lgdt(const uint64_t *gdt);
void ltr(const uint16_t *offset);
void lidt(const uint64_t *idt);
void *memset(void *dest, uint8_t val, int len);
void *memcpy(void *dest, const void *src, int len);
int memcmp(const void *str1, const void *str2, int count);

#endif /*__COMMON_H__*/
