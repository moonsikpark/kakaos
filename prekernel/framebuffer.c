#include "types.h"
#include "common.h"
#include "framebuffer.h"

framebuffer_pos_t *framebuffer_pos_frompos(int posX, int posY)
{
    framebuffer_pos_t *pos = FRAMEBUFFER_START;
    pos += (posY * 80) + posX;
    return pos;
}

uint16_t framebuffer_pos_topos(framebuffer_pos_t *pos)
{
    framebuffer_pos_t *start = FRAMEBUFFER_START;
    uint16_t diff = (uint16_t)(pos - start);

    return diff;
}

framebuffer_pos_t *framebuffer_get_cursor()
{
    uint16_t ret = 0;
    outb(0x3D4, 0x0F);
    ret |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    ret |= ((uint16_t)inb(0x3D5)) << 8;

    int posX = ret % 80;
    int posY = ret / 80;

    framebuffer_pos_t *pos = framebuffer_pos_frompos(posX, posY);
    return pos;
}

framebuffer_pos_t *framebuffer_next_line(framebuffer_pos_t *pos)
{
    framebuffer_pos_t *start = FRAMEBUFFER_START;
    uint16_t diff = (uint16_t)(pos - start);
    if (diff <= 0)
    {
        return start;
    }
    else if (diff > 80 * 24)
    {
        framebuffer_scroll();
        return framebuffer_pos_frompos(0, 25);
    }
    else
    {
        uint16_t current_line = diff / 80 + 1;
        return framebuffer_pos_frompos(0, current_line);
    }
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

    for (int i = 0; i < 80 * 25; i++)
    {
        pos->character = blank;
        pos->attribute = attribute_bw;
        pos++;
    }
}

void framebuffer_scroll()
{
    framebuffer_pos_t *pos = FRAMEBUFFER_START;
    for (int i = 0; i < 24 * 80; i++)
    {
        pos[i] = pos[i + 80];
    }
    framebuffer_move_cursor(framebuffer_pos_frompos(0, 25));
}

void framebuffer_print_string(framebuffer_pos_t *pos, const char *string)
{
    int i;
    for (i = 0; string[i] != '\0'; i++)
    {
        pos[i].character = string[i];
    }
    framebuffer_move_cursor(pos + i);
}

void framebuffer_print_colorstring(framebuffer_pos_t *pos, uint8_t forecolor, uint8_t backcolor, const char *string)
{
    uint8_t attrib = (forecolor << 4) | (backcolor & 0x0F);

    int i;
    for (i = 0; string[i] != '\0'; i++)
    {
        pos[i].character = string[i];
        pos[i].attribute = attrib;
    }
    framebuffer_move_cursor(pos + i);
}

void framebuffer_print_after(const char *string)
{
    framebuffer_pos_t *pos = framebuffer_get_cursor();

    framebuffer_print_string(pos, string);
}

void framebuffer_print_nextline(const char *string)
{
    framebuffer_pos_t *pos = framebuffer_next_line(framebuffer_get_cursor());

    framebuffer_print_string(pos, string);
}

void framebuffer_print_pflog(const char *string)
{
    framebuffer_print_nextline(string);
    framebuffer_pos_t *current = framebuffer_get_cursor();
    framebuffer_pos_t *pos = framebuffer_next_line(framebuffer_get_cursor());
    pos -= 6;
    for (; current != pos; current++)
    {
        framebuffer_print_string(current, ".");
    }
    framebuffer_print_string(pos, "[    ]");
}
void framebuffer_print_pflog_pf(int result)
{
    framebuffer_pos_t *current = framebuffer_get_cursor() - 5;
    if (result == 0)
    {
        framebuffer_print_colorstring(current, FRAMEBUFFER_COLOR_GREEN, FRAMEBUFFER_COLOR_BLACK, "PASS");
    }
    else
    {
        framebuffer_print_colorstring(current, FRAMEBUFFER_COLOR_RED, FRAMEBUFFER_COLOR_BLACK, "FAIL");
    }
}
