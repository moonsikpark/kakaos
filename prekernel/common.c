#include "types.h"
#include "common.h"

void outb(uint16_t port, uint8_t value)
{
    __asm__ __volatile__ ("outb %0, %1" :: "a" (value), "dN" (port));
}
