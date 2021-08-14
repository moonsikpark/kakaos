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

void init_cpuid_print_brand_and_model()
{
    uint32_t output[4];
    __asm__ __volatile__("mov $0x0, %eax");
    __asm__ __volatile__("cpuid");
    __asm__ __volatile__("mov %%ebx, %0"
                         : "=r"(output[0]));
    __asm__ __volatile__("mov %%edx, %0"
                         : "=r"(output[1]));
    __asm__ __volatile__("mov %%ecx, %0"
                         : "=r"(output[2]));
    output[3] = 0x0;
    framebuffer_print_after((const char *)output);
    framebuffer_print_after(" ");

    __asm__ __volatile__("mov $0x80000002 , %eax");
    __asm__ __volatile__("cpuid");
    __asm__ __volatile__("mov %%eax, %0"
                         : "=r"(output[0]));
    __asm__ __volatile__("mov %%ebx, %0"
                         : "=r"(output[1]));
    __asm__ __volatile__("mov %%ecx, %0"
                         : "=r"(output[2]));
    __asm__ __volatile__("mov %%edx, %0"
                         : "=r"(output[3]));
    framebuffer_print_after((const char *)output);

    __asm__ __volatile__("mov $0x80000003 , %eax");
    __asm__ __volatile__("cpuid");
    __asm__ __volatile__("mov %%eax, %0"
                         : "=r"(output[0]));
    __asm__ __volatile__("mov %%ebx, %0"
                         : "=r"(output[1]));
    __asm__ __volatile__("mov %%ecx, %0"
                         : "=r"(output[2]));
    __asm__ __volatile__("mov %%edx, %0"
                         : "=r"(output[3]));
    framebuffer_print_after((const char *)output);

    __asm__ __volatile__("mov $0x80000004 , %eax");
    __asm__ __volatile__("cpuid");
    __asm__ __volatile__("mov %%eax, %0"
                         : "=r"(output[0]));
    __asm__ __volatile__("mov %%ebx, %0"
                         : "=r"(output[1]));
    __asm__ __volatile__("mov %%ecx, %0"
                         : "=r"(output[2]));
    __asm__ __volatile__("mov %%edx, %0"
                         : "=r"(output[3]));
    framebuffer_print_after((const char *)output);
}

int init_cpuid_check_amd64_support()
{
    uint32_t output;
    __asm__ __volatile__("mov $0x80000001, %eax");
    __asm__ __volatile__("cpuid");
    __asm__ __volatile__("mov %%edx, %0"
                         : "=r"(output));
    if (output & (1 << 29))
    {
        return 0;
    }
    return 1;
}
