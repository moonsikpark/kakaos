#include "types.h"
#include "framebuffer.h"
#include "keyboard.h"
#include "descriptor.h"
#include "common.h"

void main()
{
    framebuffer_print_pflog_pf(0);
    framebuffer_print_nextline("Kernel started.");
    descriptor_init_gdt_table_and_tss();
    lgdt((uint64_t *)GDTR_STARTADDRESS);
    ltr((uint16_t *)GDT_TSSSEGMENT);
    descriptor_init_idt_tables();
    lidt((uint64_t *)IDTR_STARTADDRESS);
    keyboard_activate();
    keyboard_change_lock_status(false, false, false);

    uint8_t buffer[2] = {0};
    uint8_t code;
    bool flags;

    while (true)
    {
        if (keyboard_output_buffer_full())
        {
            code = keyboard_get_scan_code();

            if (keyboard_convert_scancode_to_ascii(code, &buffer[0], &flags))
            {
                if (flags & KEY_FLAGS_DOWN)
                {
                    framebuffer_print_after((const char *)buffer);

                    if (buffer[0] == '0')
                    {
                        code = code / 0;
                    }
                }
            }
        }
    }
}
