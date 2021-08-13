#ifndef __COMMON_H__
#define __COMMON_H__

#include "types.h"

void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
int init_kernel64_area_init();
uint16_t init_verify_minimum_memory();

#endif /*__COMMON_H__*/
