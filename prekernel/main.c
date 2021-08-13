#include "types.h"
#include "common.h"
#include "framebuffer.h"

void main()
{
    framebuffer_print_nextline("Prekernel started.");

    uint16_t ret = 0;
    //TODO: printf MINIMUM_MEMORY instead of hardcoding.
    framebuffer_print_nextline("Checking whether memory is over 64MB...");
    ret = init_verify_minimum_memory();
    if (ret == 0)
    {
        framebuffer_print_after("Success.");
    }
    else
    {
        //TODO: Implement printf and show current memory in megabytes (from ret).
        framebuffer_print_after("Failed. Memory too small.");
        goto loop;
    }

    framebuffer_print_nextline("Initializing kernel area...");
    ret = (uint16_t)init_kernel64_area_init();
    if (ret == 0)
    {
        framebuffer_print_after("Success.");
    }
    else
    {
        framebuffer_print_after("Failed.");
        goto loop;
    }

loop:
    while (1)
        ;
}
