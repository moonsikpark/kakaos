#include "types.h"
#include "common.h"
#include "keyboard.h"

static keymap_entry_t keyboard_keymap_table[KEY_MAPPINGTABLEMAXCOUNT] =
    {
        {KEY_NONE, KEY_NONE},
        {KEY_ESC, KEY_ESC},
        {'1', '!'},
        {'2', '@'},
        {'3', '#'},
        {'4', '$'},
        {'5', '%'},
        {'6', '^'},
        {'7', '&'},
        {'8', '*'},
        {'9', '('},
        {'0', ')'},
        {'-', '_'},
        {'=', '+'},
        {KEY_BACKSPACE, KEY_BACKSPACE},
        {KEY_TAB, KEY_TAB},
        {'q', 'Q'},
        {'w', 'W'},
        {'e', 'E'},
        {'r', 'R'},
        {'t', 'T'},
        {'y', 'Y'},
        {'u', 'U'},
        {'i', 'I'},
        {'o', 'O'},
        {'p', 'P'},
        {'[', '{'},
        {']', '}'},
        {'\n', '\n'},
        {KEY_CTRL, KEY_CTRL},
        {'a', 'A'},
        {'s', 'S'},
        {'d', 'D'},
        {'f', 'F'},
        {'g', 'G'},
        {'h', 'H'},
        {'j', 'J'},
        {'k', 'K'},
        {'l', 'L'},
        {';', ':'},
        {'\'', '\"'},
        {'`', '~'},
        {KEY_LSHIFT, KEY_LSHIFT},
        {'\\', '|'},
        {'z', 'Z'},
        {'x', 'X'},
        {'c', 'C'},
        {'v', 'V'},
        {'b', 'B'},
        {'n', 'N'},
        {'m', 'M'},
        {',', '<'},
        {'.', '>'},
        {'/', '?'},
        {KEY_RSHIFT, KEY_RSHIFT},
        {'*', '*'},
        {KEY_LALT, KEY_LALT},
        {' ', ' '},
        {KEY_CAPSLOCK, KEY_CAPSLOCK},
        {KEY_F1, KEY_F1},
        {KEY_F2, KEY_F2},
        {KEY_F3, KEY_F3},
        {KEY_F4, KEY_F4},
        {KEY_F5, KEY_F5},
        {KEY_F6, KEY_F6},
        {KEY_F7, KEY_F7},
        {KEY_F8, KEY_F8},
        {KEY_F9, KEY_F9},
        {KEY_F10, KEY_F10},
        {KEY_NUMLOCK, KEY_NUMLOCK},
        {KEY_SCROLLLOCK, KEY_SCROLLLOCK},
        {KEY_HOME, '7'},
        {KEY_UP, '8'},
        {KEY_PAGEUP, '9'},
        {'-', '-'},
        {KEY_LEFT, '4'},
        {KEY_CENTER, '5'},
        {KEY_RIGHT, '6'},
        {'+', '+'},
        {KEY_END, '1'},
        {KEY_DOWN, '2'},
        {KEY_PAGEDOWN, '3'},
        {KEY_INS, '0'},
        {KEY_DEL, '.'},
        {KEY_NONE, KEY_NONE},
        {KEY_NONE, KEY_NONE},
        {KEY_NONE, KEY_NONE},
        {KEY_F11, KEY_F11},
        {KEY_F12, KEY_F12}};

static keyboardstate_t keyboard_state = {0};

bool keyboard_output_buffer_full()
{
    if (inb(0x64) & 0x01)
    {
        return true;
    }
    return false;
}

bool keyboard_input_buffer_full()
{
    if (inb(0x64) & 0x02)
    {
        return true;
    }
    return false;
}

void keyboard_wait_input_buffer()
{
    for (int i = 0; i < 0xffff; i++)
    {
        if (!keyboard_input_buffer_full())
            break;
    }
}

void keyboard_wait_output_buffer()
{
    for (int i = 0; i < 0xffff; i++)
    {
        if (!keyboard_output_buffer_full())
            break;
    }
}

int keyboard_activate()
{
    outb(0x64, 0xAE);

    keyboard_wait_input_buffer();

    outb(0x60, 0xF4);

    for (int i = 0; i < 100; i++)
    {
        keyboard_wait_output_buffer();
        if (inb(0x60) == 0xFA)
        {
            return true;
        }
    }
    return false;
}

uint8_t keyboard_get_scan_code()
{
    while (true)
    {
        if (keyboard_output_buffer_full())
        {
            return inb(0x60);
        }
    }
}

int keyboard_change_lock_status(bool capslock, bool numlock, bool scrolllock)
{
    bool success = false;
    keyboard_wait_input_buffer();
    outb(0x60, 0xED);

    keyboard_wait_input_buffer();

    for (int i = 0; i < 100; i++)
    {
        keyboard_wait_output_buffer();
        if (inb(0x60) == 0xfa)
        {
            success = true;
            break;
        }
    }
    if (!success)
    {
        goto fail;
    }

    outb(0x60, (capslock << 2) | (numlock << 1) | scrolllock);
    keyboard_wait_input_buffer();

    success = false;
    for (int i = 0; i < 100; i++)
    {
        keyboard_wait_output_buffer();
        if (inb(0x60) == 0xfa)
        {
            success = true;
            break;
        }
    }
    if (!success)
    {
        goto fail;
    }

    return true;

fail:
    return false;
}

int keyboard_enable_a20_gate()
{
    outb(0x64, 0xd0);

    keyboard_wait_output_buffer();

    uint8_t output = inb(0x60);
    output |= 0x01;

    keyboard_wait_input_buffer();

    outb(0x64, 0xd1);
    outb(0x60, output);

    return 0;
}

int keyboard_reset_system()
{
    keyboard_wait_input_buffer();

    outb(0x64, 0xd1);
    outb(0x60, 0x00);

    while (true)
        ;
}

bool keyboard_scancode_is_alphabet(uint8_t code)
{
    if (('a' <= keyboard_keymap_table[code].normal_code) &&
        (keyboard_keymap_table[code].normal_code <= 'z'))
    {
        return true;
    }
    return false;
}

bool keyboard_scancode_is_number_or_symbol(uint8_t code)
{
    if ((2 <= code) && (code <= 53) && !keyboard_scancode_is_alphabet(code))
    {
        return true;
    }
    return false;
}

bool keyboard_scancode_is_numpad(uint8_t code)
{
    if ((71 <= code) && (code <= 83))
    {
        return true;
    }
    return false;
}

bool keyboard_should_use_combined_code(uint8_t code)
{
    uint8_t scancode_down = code & 0x7f;

    if (keyboard_scancode_is_alphabet(code))
    {
        return keyboard_state.shift ^ keyboard_state.capslock;
    }
    else if (keyboard_scancode_is_number_or_symbol(code))
    {
        return keyboard_state.shift;
    }
    else if (keyboard_scancode_is_numpad(code) && !keyboard_state.extended_code)
    {
        return keyboard_state.numlock;
    }

    return false;
}

int keyboard_update_combination_key_and_state(uint8_t scancode)
{
    bool is_down;
    uint8_t down_scan_code;
    bool keyboard_state_changed = false;

    if (scancode & 0x80)
    {
        is_down = false;
        down_scan_code = scancode & 0x7f;
    }
    else
    {
        is_down = true;
        down_scan_code = scancode;
    }

    if ((scancode == 42) || (scancode == 54))
    {
        keyboard_state.shift = is_down;
    }
    else if ((scancode == 58) && is_down)
    {

        keyboard_state.capslock ^= true;
        keyboard_state_changed = true;
    }
    else if ((scancode == 69) && is_down)
    {
        keyboard_state.numlock ^= true;
        keyboard_state_changed = true;
    }
    else if ((scancode == 70) && is_down)
    {
        keyboard_state.scrlock ^= true;
        keyboard_state_changed = true;
    }

    if (keyboard_state_changed)
    {
        keyboard_change_lock_status(keyboard_state.capslock, keyboard_state.numlock, keyboard_state.scrlock);
    }

    return 0;
}

bool keyboard_convert_scancode_to_ascii(uint8_t scancode, uint8_t *asciicode, bool *flags)
{
    if (keyboard_state.skip_count > 0)
    {
        keyboard_state.skip_count--;
        return false;
    }

    if (scancode == 0xE1)
    {
        *asciicode = KEY_PAUSE;
        *flags = KEY_FLAGS_DOWN;
        keyboard_state.skip_count = KEY_PAUSE_SKIPCOUNT;
        return true;
    }
    else if (scancode == 0xE0)
    {
        keyboard_state.extended_code = true;
        return false;
    }

    if (keyboard_should_use_combined_code(scancode))
    {
        *asciicode = keyboard_keymap_table[scancode & 0x7F].combined_code;
    }
    else
    {
        *asciicode = keyboard_keymap_table[scancode & 0x7F].normal_code;
    }

    if (keyboard_state.extended_code)
    {
        *flags = KEY_FLAGS_EXTENDEDKEY;
        keyboard_state.extended_code = false;
    }
    else
    {
        *flags = 0;
    }

    if ((scancode & 0x80) == 0)
    {
        *flags |= KEY_FLAGS_DOWN;
    }

    keyboard_update_combination_key_and_state(scancode);
    return true;
}
