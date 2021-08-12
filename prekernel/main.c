#include "types.h"
#include "framebuffer.h"

void main()
{
    framebuffer_print_nextline("Prekernel started.");
    while (1)
        ;
}
