#include "types.h"

void main()
{
    print_string(0, 5, "Prekernel started." );
    while(1);
}

void print_string( int posX, int posY, const char *string)
{
    screen_t *screen = (screen_t *) 0xB8000;
    
    screen += ( posY * 80 ) + posX;
    
    for(int i = 0 ; string[i] != 0 ; i++ )
    {
        screen[i].character = string[i];
    }
}
