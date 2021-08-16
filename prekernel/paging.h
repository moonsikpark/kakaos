#ifndef __PAGING_H__
#define __PAGING_H__

#include "types.h"

#define PAGE_FLAGS_P 0x00000001
#define PAGE_FLAGS_RW 0x00000002
#define PAGE_FLAGS_US 0x00000004
#define PAGE_FLAGS_PWT 0x00000008
#define PAGE_FLAGS_PCD 0x00000010
#define PAGE_FLAGS_A 0x00000020
#define PAGE_FLAGS_D 0x00000040
#define PAGE_FLAGS_PS 0x00000080
#define PAGE_FLAGS_G 0x00000100
#define PAGE_FLAGS_PAT 0x00001000

#define PAGE_FLAGS_ED 0x80000000

#define PAGE_FLAGS_DEFAULT (PAGE_FLAGS_P | PAGE_FLAGS_RW)

#define PAGE_TABLESIZE 0x1000
#define PAGE_MAXENTRYCOUNT 512
#define PAGE_DEFAULTSIZE 0x200000

typedef struct pagetable_entry_t
{
    uint32_t lower_baseaddr_and_attr;
    uint32_t upper_baseaddr_and_ed;
} __attribute__((aligned(1), packed)) pagetable_entry_t;

int init_page_tables();

#endif /*__PAGING_H__*/
