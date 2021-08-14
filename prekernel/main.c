#include "types.h"
#include "common.h"
#include "framebuffer.h"

void main()
{
    framebuffer_print_nextline("Prekernel started.");

    uint16_t ret = 0;
    //TODO: printf MINIMUM_MEMORY instead of hardcoding.
    framebuffer_print_pflog("Checking whether memory is over 64MB");
    ret = init_verify_minimum_memory();
    framebuffer_print_pflog_pf(ret);
    if (ret)
    {
        //TODO: Implement printf and show current memory in megabytes (from ret).
        framebuffer_print_nextline("Memory too small.");
        goto loop;
    }

    framebuffer_print_pflog("Initializing kernel area");
    ret = (uint16_t)init_kernel64_area_init();
    framebuffer_print_pflog_pf(ret);
    if (ret)
    {
        framebuffer_print_nextline("Failed to initialize kernel area.");
        goto loop;
    }

loop:
    while (1)
        ;
}
