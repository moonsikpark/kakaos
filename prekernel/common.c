#include "types.h"
#include "common.h"
#include "framebuffer.h"

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
