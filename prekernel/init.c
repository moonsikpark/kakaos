#include "types.h"
#include "framebuffer.h"

const uint16_t MINIMUM_MEMORY = 64; // 64MB

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

int init_load_kernel_image()
{
    uint16_t size_all = *((uint16_t *)0x7C05);
    uint16_t size_prekernel = *((uint16_t *)0x7C07);

    uint32_t *source_addr = (uint32_t *)(0x10000 + (size_prekernel * 512));
    uint32_t *dest_addr = (uint32_t *)0x200000;

    for (int i = 0; i < 512 * (size_all - size_prekernel) / 4; i++)
    {
        *dest_addr = *source_addr;
        dest_addr++;
        source_addr++;
    }

    return 0;
}
