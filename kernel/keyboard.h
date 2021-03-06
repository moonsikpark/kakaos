#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include "types.h"

#define KEY_FLAGS_UP 0x00
#define KEY_FLAGS_DOWN 0x01
#define KEY_FLAGS_EXTENDEDKEY 0x02

#define KEY_MAPPINGTABLEMAXCOUNT 89

#define KEY_NONE 0x00
#define KEY_ENTER '\n'
#define KEY_TAB '\t'
#define KEY_ESC 0x1B
#define KEY_BACKSPACE 0x08

#define KEY_CTRL 0x81
#define KEY_LSHIFT 0x82
#define KEY_RSHIFT 0x83
#define KEY_PRINTSCREEN 0x84
#define KEY_LALT 0x85
#define KEY_CAPSLOCK 0x86
#define KEY_F1 0x87
#define KEY_F2 0x88
#define KEY_F3 0x89
#define KEY_F4 0x8A
#define KEY_F5 0x8B
#define KEY_F6 0x8C
#define KEY_F7 0x8D
#define KEY_F8 0x8E
#define KEY_F9 0x8F
#define KEY_F10 0x90
#define KEY_NUMLOCK 0x91
#define KEY_SCROLLLOCK 0x92
#define KEY_HOME 0x93
#define KEY_UP 0x94
#define KEY_PAGEUP 0x95
#define KEY_LEFT 0x96
#define KEY_CENTER 0x97
#define KEY_RIGHT 0x98
#define KEY_END 0x99
#define KEY_DOWN 0x9A
#define KEY_PAGEDOWN 0x9B
#define KEY_INS 0x9C
#define KEY_DEL 0x9D
#define KEY_F11 0x9E
#define KEY_F12 0x9F
#define KEY_PAUSE 0xA0
#define KEY_PAUSE_SKIPCOUNT 2

typedef struct keymap_entry_t
{
    uint8_t normal_code;
    uint8_t combined_code;
} __attribute__((aligned(1), packed)) keymap_entry_t;

typedef struct keyboardstate_t
{
    bool shift;
    bool capslock;
    bool numlock;
    bool scrlock;
    bool extended_code;

    int skip_count;
} __attribute__((aligned(1), packed)) keyboardstate_t;

int keyboard_activate();
int keyboard_change_lock_status(bool capslock, bool numlock, bool scrolllock);
bool keyboard_output_buffer_full();
uint8_t keyboard_get_scan_code();
bool keyboard_convert_scancode_to_ascii(uint8_t scancode, uint8_t *asciicode, bool *flags);

#endif /*__KEYBOARD_H__*/
