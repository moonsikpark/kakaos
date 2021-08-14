#include "types.h"
#include "common.h"
#include "framebuffer.h"

const uint16_t MINIMUM_MEMORY = 64; // 64MB

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

uint16_t init_verify_minimum_memory()
{
    // Start from 0x100000 (1MB)
    uint16_t memory = 1;
    uint32_t *addr = (uint32_t *)0x100000;

    for (; memory < MINIMUM_MEMORY; memory++)
    {
        *addr = 0xdeadbeef;
        if (*addr != 0xdeadbeef)
        {
            return memory;
        }
        addr += (0x100000 / 4);
    }
    return (uint16_t)0;
}

int init_kernel64_area_init()
{
    uint16_t *addr = (uint16_t *)0x100000;

    while (addr > (uint16_t *)0x600000)
    {
        *addr = 0x00;
        if (*addr != 0x00)
            return 1;

        addr++;
    }
    return 0;
}
