#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#define FRAMEBUFFER_START (framebuffer_pos_t *)0xB8000

#define FRAMEBUFFER_WIDTH 80
#define FRAMEBUFFER_HEIGHT 25

#define FRAMEBUFFER_COLOR_BLACK 0
#define FRAMEBUFFER_COLOR_BLUE 1
#define FRAMEBUFFER_COLOR_GREEN 2
#define FRAMEBUFFER_COLOR_CYAN 3
#define FRAMEBUFFER_COLOR_RED 4
#define FRAMEBUFFER_COLOR_MAGENTA 5
#define FRAMEBUFFER_COLOR_BROWN 6
#define FRAMEBUFFER_COLOR_LIGHTGREY 7
#define FRAMEBUFFER_COLOR_DARKGREY 8
#define FRAMEBUFFER_COLOR_LIGHTBLUE 9
#define FRAMEBUFFER_COLOR_LIGHTGREEN 10
#define FRAMEBUFFER_COLOR_LIGHTCYAN 11
#define FRAMEBUFFER_COLOR_LIGHTRED 12
#define FRAMEBUFFER_COLOR_LIGHTMAGENTA 13
#define FRAMEBUFFER_COLOR_LIGHTBROWN 14
#define FRAMEBUFFER_COLOR_WHITE 15

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
void framebuffer_print_string(framebuffer_pos_t *pos, const char *string);
void framebuffer_print_colorstring(framebuffer_pos_t *pos, uint8_t forecolor, uint8_t backcolor, const char *string);
void framebuffer_print_after(const char *string);
void framebuffer_print_nextline(const char *string);
void framebuffer_print_pflog(const char *string);
void framebuffer_print_pflog_pf(int result);

#endif /*__FRAMEBUFFER_H__*/
