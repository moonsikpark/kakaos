#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__


typedef struct framebuffer_pos_t
{
    volatile uint8_t character;
    volatile uint8_t attribute;
} __attribute__((aligned(1), packed)) framebuffer_pos_t;

#define FRAMEBUFFER() (framebuffer_pos_t *) 0xB8000

void framebuffer_move_cursor(int posX, int posY);
void framebuffer_clear_screen();
void framebuffer_print_string(int posX, int posY, const char *string);

#endif /*__FRAMEBUFFER_H__*/
