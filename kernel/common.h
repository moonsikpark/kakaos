#ifndef __COMMON_H__
#define __COMMON_H__

#include "types.h"

void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);

#endif /*__COMMON_H__*/
