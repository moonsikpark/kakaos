#include "paging.h"

int init_page_tables()
{
    pagetable_entry_t *pml4_table = (pagetable_entry_t *)0x100000;
    init_set_pagetable_entry(pml4_table, 0x101000, PAGE_FLAGS_DEFAULT, 0x00, 0);
    for (int i = 1; i < PAGE_MAXENTRYCOUNT; i++)
    {
        init_set_pagetable_entry(pml4_table + i, 0, 0, 0, 0);
    }

    pagetable_entry_t *pdp_table = (pagetable_entry_t *)0x101000;
    for (int i = 0; i < 64; i++)
    {
        init_set_pagetable_entry(pdp_table + i, 0x102000 + (i * PAGE_TABLESIZE), PAGE_FLAGS_DEFAULT, 0, 0);
    }
    for (int i = 64; i < PAGE_MAXENTRYCOUNT; i++)
    {
        init_set_pagetable_entry(pdp_table + i, 0, 0, 0, 0);
    }

    pagetable_entry_t *pd_table = (pagetable_entry_t *)0x102000;
    for (int i = 0; i < PAGE_MAXENTRYCOUNT * 64; i++)
    {
        init_set_pagetable_entry(pd_table + i, i * PAGE_DEFAULTSIZE, PAGE_FLAGS_DEFAULT | PAGE_FLAGS_PS, (i * (PAGE_DEFAULTSIZE >> 20)) >> 12, 0);
    }

    return 0;
}

void init_set_pagetable_entry(pagetable_entry_t *pstEntry, uint32_t lower_baseaddr, uint32_t lower_flags, uint32_t upper_baseaddr, uint32_t upper_flags)
{
    pstEntry->lower_baseaddr_and_attr = lower_baseaddr | lower_flags;
    pstEntry->upper_baseaddr_and_ed = (upper_baseaddr & 0xFF) | upper_flags;
}
