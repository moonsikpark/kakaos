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

    framebuffer_print_nextline("CPU Information: ");
    init_cpuid_print_brand_and_model();

    framebuffer_print_pflog("Checking amd64 support");
    ret = (uint16_t)init_cpuid_check_amd64_support();
    framebuffer_print_pflog_pf(ret);
    if (ret)
    {
        framebuffer_print_nextline("Processor does not support amd64.");
        goto loop;
    }

loop:
    while (1)
        ;
}
