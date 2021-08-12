#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#define FRAMEBUFFER_START (framebuffer_pos_t *) 0xB8000

typedef struct framebuffer_pos_t
{
    volatile uint8_t character;
    volatile uint8_t attribute;
} __attribute__((aligned(1), packed)) framebuffer_pos_t;

framebuffer_pos_t *framebuffer_pos_frompos(int posX, int posY);
uint16_t framebuffer_pos_topos(framebuffer_pos_t *pos);
framebuffer_pos_t *framebuffer_get_cursor();
void framebuffer_move_cursor(framebuffer_pos_t *pos);
void framebuffer_clear_screen();
void framebuffer_scroll();
void framebuffer_print_string(int posX, int posY, const char *string);

#endif /*__FRAMEBUFFER_H__*/
