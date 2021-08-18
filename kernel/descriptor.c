#include "common.h"
#include "framebuffer.h"
#include "descriptor.h"

void descriptor_init_gdt_table_and_tss()
{
    gdtr_t *gdtr = (gdtr_t *)GDTR_STARTADDRESS;

    gdt_entry_8_t *entry = (gdt_entry_8_t *)(GDTR_STARTADDRESS + sizeof(gdtr_t));

    gdtr->limit = GDT_TABLESIZE - 1;
    gdtr->base_addr = (uint64_t)entry;
    tss_data_t *tss = (tss_data_t *)((uint64_t)entry + GDT_TABLESIZE);
    descriptor_set_gdt_entry_8(&(entry[0]), 0, 0, 0, 0, 0);
    descriptor_set_gdt_entry_8(&(entry[1]), 0, 0xFFFFF, GDT_FLAGS_UPPER_CODE, GDT_FLAGS_LOWER_KERNELCODE, GDT_TYPE_CODE);
    descriptor_set_gdt_entry_8(&(entry[2]), 0, 0xFFFFF, GDT_FLAGS_UPPER_DATA, GDT_FLAGS_LOWER_KERNELDATA, GDT_TYPE_DATA);
    descriptor_set_gdt_entry_16((gdt_entry_16_t *)&(entry[3]), (uint64_t)tss, sizeof(tss_data_t) - 1, GDT_FLAGS_UPPER_TSS, GDT_FLAGS_LOWER_TSS, GDT_TYPE_TSS);

    descriptor_init_tss_segment(tss);
}

void descriptor_set_gdt_entry_8(gdt_entry_8_t *entry, uint32_t base_addr, uint32_t limit, uint8_t upper_flags, uint8_t lower_flags, uint8_t type)
{
    entry->lower_limit = limit & 0xFFFF;
    entry->lower_base_addr = base_addr & 0xFFFF;
    entry->upper_base_addr1 = (base_addr >> 16) & 0xFF;
    entry->type_and_lower_flag = lower_flags | type;
    entry->upper_limit_and_upper_flag = ((limit >> 16) & 0xFF) |
                                        upper_flags;
    entry->upper_base_addr2 = (base_addr >> 24) & 0xFF;
}

void descriptor_set_gdt_entry_16(gdt_entry_16_t *entry, uint64_t base_addr, uint32_t limit, uint8_t upper_flags, uint8_t lower_flags, uint8_t type)
{
    entry->lower_limit = limit & 0xFFFF;
    entry->lower_base_addr = base_addr & 0xFFFF;
    entry->middle_base_addr1 = (base_addr >> 16) & 0xFF;
    entry->type_and_lower_flag = lower_flags | type;
    entry->upper_limit_and_flag = ((limit >> 16) & 0xFF) |
                                  upper_flags;
    entry->middle_base_addr2 = (base_addr >> 24) & 0xFF;
    entry->upper_base_addr = base_addr >> 32;
    entry->reserved = 0;
}

void descriptor_init_tss_segment(tss_data_t *tss)
{
    memset(tss, 0, sizeof(tss_data_t));
    tss->ist[0] = IST_STARTADDRESS + IST_SIZE;
    tss->iomap_base_addr = 0xFFFF;
}

void descriptor_init_idt_tables()
{
    gdtr_t *idtr = (gdtr_t *)IDTR_STARTADDRESS;
    idt_entry_t *entry = (idt_entry_t *)(IDTR_STARTADDRESS + sizeof(gdtr_t));
    idtr->base_addr = (uint64_t)entry;
    idtr->limit = IDT_TABLESIZE - 1;

    for (int i = 0; i < IDT_MAXENTRYCOUNT; i++)
    {
        descriptor_set_idt_entry(&(entry[i]), descriptor_test_handler, 0x08, IDT_FLAGS_IST1, IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
    }
}

void descriptor_set_idt_entry(idt_entry_t *entry, void *handler, uint16_t selector, uint8_t ist, uint8_t flags, uint8_t type)
{
    entry->lower_base_addr = (uint64_t)handler & 0xffff;
    entry->segment_selector = selector;
    entry->ist = ist & 0x3;
    entry->type_and_flags = type | flags;
    entry->middle_base_addr = ((uint64_t)handler >> 16) & 0xffff;
    entry->upper_base_addr = (uint64_t)handler >> 32;
    entry->reserved = 0;
}

void descriptor_test_handler()
{
    framebuffer_print_nextline("Intrrupt caught. halting.");

    while (1)
        ;
}
