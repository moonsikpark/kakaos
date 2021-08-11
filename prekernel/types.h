#ifndef __TYPES_H__
#define __TYPES_H__

typedef unsigned char uint_t;
typedef unsigned short uint2_t;
typedef unsigned int uint4_t;
typedef unsigned long uint8_t;

typedef struct screen_t
{
    uint_t character;
    uint_t attribute;
} __attribute__((aligned(1), packed)) screen_t;

void print_string(int posX, int posY, const char *string);
void main();

#endif /*__TYPES_H__*/
