#ifndef __DESCRIPTOR_H__
#define __DESCRIPTOR_H__

#include "types.h"

#define GDT_TYPE_CODE 0x0A
#define GDT_TYPE_DATA 0x02
#define GDT_TYPE_TSS 0x09
#define GDT_FLAGS_LOWER_S 0x10
#define GDT_FLAGS_LOWER_DPL0 0x00
#define GDT_FLAGS_LOWER_DPL1 0x20
#define GDT_FLAGS_LOWER_DPL2 0x40
#define GDT_FLAGS_LOWER_DPL3 0x60
#define GDT_FLAGS_LOWER_P 0x80
#define GDT_FLAGS_UPPER_L 0x20
#define GDT_FLAGS_UPPER_DB 0x40
#define GDT_FLAGS_UPPER_G 0x80

#define GDT_FLAGS_LOWER_KERNELCODE (GDT_TYPE_CODE | GDT_FLAGS_LOWER_S | \
                                    GDT_FLAGS_LOWER_DPL0 | GDT_FLAGS_LOWER_P)
#define GDT_FLAGS_LOWER_KERNELDATA (GDT_TYPE_DATA | GDT_FLAGS_LOWER_S | \
                                    GDT_FLAGS_LOWER_DPL0 | GDT_FLAGS_LOWER_P)
#define GDT_FLAGS_LOWER_TSS (GDT_FLAGS_LOWER_DPL0 | GDT_FLAGS_LOWER_P)
#define GDT_FLAGS_LOWER_USERCODE (GDT_TYPE_CODE | GDT_FLAGS_LOWER_S | \
                                  GDT_FLAGS_LOWER_DPL3 | GDT_FLAGS_LOWER_P)
#define GDT_FLAGS_LOWER_USERDATA (GDT_TYPE_DATA | GDT_FLAGS_LOWER_S | \
                                  GDT_FLAGS_LOWER_DPL3 | GDT_FLAGS_LOWER_P)

#define GDT_FLAGS_UPPER_CODE (GDT_FLAGS_UPPER_G | GDT_FLAGS_UPPER_L)
#define GDT_FLAGS_UPPER_DATA (GDT_FLAGS_UPPER_G | GDT_FLAGS_UPPER_L)
#define GDT_FLAGS_UPPER_TSS (GDT_FLAGS_UPPER_G)

#define GDT_KERNELCODESEGMENT 0x08
#define GDT_KERNELDATASEGMENT 0x10
#define GDT_TSSSEGMENT 0x18

#define GDTR_STARTADDRESS 0x142000
#define GDT_MAXENTRY8COUNT 3
#define GDT_MAXENTRY16COUNT 1

#define IDT_TYPE_INTERRUPT 0x0E
#define IDT_TYPE_TRAP 0x0F
#define IDT_FLAGS_DPL0 0x00
#define IDT_FLAGS_DPL1 0x20
#define IDT_FLAGS_DPL2 0x40
#define IDT_FLAGS_DPL3 0x60
#define IDT_FLAGS_P 0x80
#define IDT_FLAGS_IST0 0
#define IDT_FLAGS_IST1 1

#define IDT_FLAGS_KERNEL (IDT_FLAGS_DPL0 | IDT_FLAGS_P)
#define IDT_FLAGS_USER (IDT_FLAGS_DPL3 | IDT_FLAGS_P)

#define IDT_MAXENTRYCOUNT 100

#define IST_STARTADDRESS 0x700000
#define IST_SIZE 0x100000

typedef struct gdtr_t
{
    uint16_t limit;
    uint64_t base_addr;
    uint16_t padding1;
    uint32_t padding2;
} __attribute__((aligned(1), packed)) gdtr_t;

typedef struct gdt_entry_8_t
{
    uint16_t lower_limit;
    uint16_t lower_base_addr;
    uint8_t upper_base_addr1;
    uint8_t type_and_lower_flag;
    uint8_t upper_limit_and_upper_flag;
    uint8_t upper_base_addr2;
} __attribute__((aligned(1), packed)) gdt_entry_8_t;

typedef struct gdt_entry_16_t
{
    uint16_t lower_limit;
    uint16_t lower_base_addr;
    uint8_t middle_base_addr1;
    uint8_t type_and_lower_flag;
    uint8_t upper_limit_and_flag;
    uint8_t middle_base_addr2;
    uint32_t upper_base_addr;
    uint32_t reserved;
} __attribute__((aligned(1), packed)) gdt_entry_16_t;

typedef struct tss_data_t
{
    uint32_t reserved1;
    uint64_t rsp[3];
    uint64_t reserved2;
    uint64_t ist[7];
    uint64_t reserved3;
    uint16_t reserved4;
    uint16_t iomap_base_addr;
} __attribute__((aligned(1), packed)) tss_data_t;

typedef struct idt_entry_t
{
    uint16_t lower_base_addr;
    uint16_t segment_selector;
    uint8_t ist;
    uint8_t type_and_flags;
    uint16_t middle_base_addr;
    uint32_t upper_base_addr;
    uint32_t reserved;
} __attribute__((aligned(1), packed)) idt_entry_t;

#define GDT_TABLESIZE ((sizeof(gdt_entry_8_t) * GDT_MAXENTRY8COUNT) + \
                       (sizeof(gdt_entry_16_t) * GDT_MAXENTRY16COUNT))
#define TSS_SEGMENTSIZE (sizeof(tss_data_t))
#define IDTR_STARTADDRESS (GDTR_STARTADDRESS + sizeof(gdtr_t) + \
                           GDT_TABLESIZE + TSS_SEGMENTSIZE)
#define IDT_STARTADDRESS (IDTR_STARTADDRESS + sizeof(gdtr_t))
#define IDT_TABLESIZE (IDT_MAXENTRYCOUNT * sizeof(idt_entry_t))

void descriptor_init_gdt_table_and_tss();
void descriptor_set_gdt_entry_8(gdt_entry_8_t *entry, uint32_t base_addr, uint32_t limit, uint8_t upper_flags, uint8_t lower_flags, uint8_t type);
void descriptor_set_gdt_entry_16(gdt_entry_16_t *entry, uint64_t base_addr, uint32_t limit, uint8_t upper_flags, uint8_t lower_flags, uint8_t type);
void descriptor_init_tss_segment(tss_data_t *tss);
void descriptor_init_idt_tables();
void descriptor_set_idt_entry(idt_entry_t *entry, void *handler, uint16_t selector, uint8_t ist, uint8_t flags, uint8_t type);
void descriptor_test_handler();

#endif
