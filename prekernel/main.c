#include "types.h"
#include "framebuffer.h"

void main()
{
    framebuffer_print_string(0, 5, "Prekernel started.");
    framebuffer_clear_screen();
    while(1);
}
