#include "types.h"
#include "common.h"

void outb(uint16_t port, uint8_t value)
{
    __asm__ __volatile__("outb %0, %1" ::"a"(value), "dN"(port));
}

uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ __volatile__("inb %1, %0"
                         : "=a"(ret)
                         : "dN"(port));
    return ret;
}

void lgdt(const uint64_t *gdt)
{
    __asm__ __volatile__("lgdt %0" ::"m"(*gdt));
}

void ltr(const uint16_t *offset)
{
    __asm__ __volatile__("ltr %0" ::"qm"(offset));
}

void lidt(const uint64_t *idt)
{
    __asm__ __volatile__("lidt %0" ::"m"(*idt));
}

void *memset(void *dest, uint8_t val, int len)
{
    unsigned char *ptr = dest;
    while (len-- > 0)
        *ptr++ = val;
    return dest;
}

void *memcpy(void *dest, const void *src, int len)
{
    char *d = dest;
    const char *s = src;
    while (len--)
        *d++ = *s++;
    return dest;
}

int memcmp(const void *str1, const void *str2, int count)
{
    const unsigned char *s1 = str1;
    const unsigned char *s2 = str2;

    while (count-- > 0)
    {
        if (*s1++ != *s2++)
            return s1[-1] < s2[-1] ? -1 : 1;
    }
    return 0;
}
