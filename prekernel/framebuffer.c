#include "types.h"
#include "common.h"
#include "framebuffer.h"

framebuffer_pos_t *framebuffer_pos_frompos(int posX, int posY)
{
    framebuffer_pos_t *pos = FRAMEBUFFER_START;
    pos += ( posY * 80 ) + posX;
    return pos;
}

uint16_t framebuffer_pos_topos(framebuffer_pos_t *pos)
{
    framebuffer_pos_t *start = FRAMEBUFFER_START;
    uint16_t diff = (uint16_t) (pos - start);

    return diff;
}

framebuffer_pos_t *framebuffer_get_cursor()
{
    outb(0x3D4, 0x0F);
    int posX = (uint8_t) inb(0x3D5);
    outb(0x3D4, 0x0E);
    int posY = (uint8_t) inb(0x3D5);

    framebuffer_pos_t *pos = framebuffer_pos_frompos(posX, posY);
    return pos;
}

void framebuffer_move_cursor(framebuffer_pos_t *pos)
{
    uint16_t cursor_location = framebuffer_pos_topos(pos);

    outb(0x3d4, 14); // High cursor
    outb(0x3d5, cursor_location >> 8);
    outb(0x3d4, 15); // Low cursor
    outb(0x3d5, cursor_location);
}

void framebuffer_clear_screen()
{
    framebuffer_pos_t *pos = FRAMEBUFFER_START;
    framebuffer_move_cursor(pos);

    uint8_t blank = 0x20;
    uint8_t attribute_bw = (0 << 4) | (15 & 0x0F);

    for (int i = 0; i < 80 * 25; i++) {
        pos->character = blank;
        pos->attribute = attribute_bw;
        pos++;
    }
}

void framebuffer_scroll()
{
    framebuffer_pos_t *pos = FRAMEBUFFER_START;
    for(int i = 80; i < 24 * 80; i++) {
        pos[i] = pos[i + 80];
    }
    framebuffer_move_cursor(framebuffer_pos_frompos(0, 25));
}

void framebuffer_print_string(int posX, int posY, const char *string)
{
    framebuffer_pos_t *pos = framebuffer_pos_frompos(posX, posY);

    for(int i = 0 ; string[i] != 0 ; i++ )
    {
        pos[i].character = string[i];
    }
}
