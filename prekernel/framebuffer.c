#include "types.h"
#include "common.h"
#include "framebuffer.h"

void framebuffer_move_cursor(int posX, int posY)
{
    uint16_t cursor_location = posY * 80 + posX;

    outb(0x3d4, 14); // High cursor
    outb(0x3d5, cursor_location >> 8);
    outb(0x3d4, 15); // Low cursor
    outb(0x3d5, cursor_location);
}

void framebuffer_clear_screen()
{
    framebuffer_pos_t *pos = FRAMEBUFFER();
    uint8_t blank = 0x20;
    uint8_t attribute_bw = (0 << 4) | (15 & 0x0F);

    for (int i = 0; i < 80 * 25; i++) {
        pos->character = blank;
        pos->attribute = attribute_bw;
        pos++;
    }

    framebuffer_move_cursor(0, 0);
}

void framebuffer_print_string(int posX, int posY, const char *string)
{
    framebuffer_pos_t *pos = FRAMEBUFFER();

    pos += ( posY * 80 ) + posX;

    
    for(int i = 0 ; string[i] != 0 ; i++ )
    {
        pos[i].character = string[i];
    }
}

