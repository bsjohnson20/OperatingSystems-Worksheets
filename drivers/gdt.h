// gdt

#ifndef GDT_H
#define GDT_H

#include "vga.h"


// #include "gdt2.h"
#include "serial.h"
#include "vga.h"
#include "common.h"
#include <stdint.h>


struct gdt_entry
{
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char access;
    unsigned char granularity;
    unsigned char base_high;
};

struct gdt
{
    struct gdt_entry entries[3];
    unsigned short size;
    unsigned int address;
} __attribute__((packed));

extern void init_gdt();
extern void switch_to_protected_mode();
// void gdt_flush();

#endif // GDT_H
