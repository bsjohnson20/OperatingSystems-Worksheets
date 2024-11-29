// #ifndef INTERRUPTS_H
// #define INTERRUPTS_H

#ifndef INCLUDE_INTERRUPTS
#define INCLUDE_INTERRUPTS
#include <stdint.h>

#include "interrupts.h"



struct IDT
{
    uint16_t size;
    uint32_t address;
} __attribute__((packed));
struct IDTDescriptor
{
    /* The lowest 32 bits */
    uint16_t offset_low;       // offset bits 0..15
    uint16_t segment_selector; // a code segment selector in GDT or LDT
    /* The highest 32 bits */
    uint8_t reserved;      // Just 0.
    uint8_t type_and_attr; // type and attributes
    uint16_t offset_high;  // offset bits 16..31
} __attribute__((packed));
struct cpu_state
{
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
} __attribute__((packed));
struct stack_state
{
    uint32_t error_code;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
} __attribute__((packed));
extern void interrupt_handler(struct cpu_state cpu, uint32_t interrupt, struct stack_state stack);
extern void interrupts_install_idt();
// Wrappers around ASM.
// void load_idt(uint32_t idt_address);
void interrupt_handler_33();
void interrupt_handler_7();
void interrupt_handler_8();
void interrupt_handler_1();

// void load_idt(uint32_t idt_address);

#endif /* INCLUDE_INTERRUPTS */

