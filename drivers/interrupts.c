#include "interrupts.h"
#include "pic.h"
#include "io.h"
#include "vga.h"
#include "keyboard.h"
#include "serial.h"

#define INTERRUPTS_DESCRIPTOR_COUNT 256
#define INTERRUPTS_KEYBOARD 33
#define INPUT_BUFFER_SIZE 256

    uint8_t input_buffer[INPUT_BUFFER_SIZE];
uint8_t buffer_index = 0;

struct IDTDescriptor idt_descriptors[INTERRUPTS_DESCRIPTOR_COUNT];
struct IDT idt;

uint32_t BUFFER_COUNT;

void interrupts_init_descriptor(int32_t index, uint32_t address)
{
    idt_descriptors[index].offset_high = (address >> 16) & 0xFFFF; // offset bits 0..15
    idt_descriptors[index].offset_low = (address & 0xFFFF);        // offset bits 16..31

    idt_descriptors[index].segment_selector = 0x9A; // The second (code) segment selector in GDT: one segment is 64b.
    idt_descriptors[index].reserved = 0x00;         // Reserved.

    /*
       Bit:     | 31              16 | 15 | 14 13 | 12 | 11     10 9 8   | 7 6 5 | 4 3 2 1 0 |
       Content: | offset high        | P  | DPL   | S  | D and  GateType | 0 0 0 | reserved
        P	If the handler is present in memory or not (1 = present, 0 = not present). Set to 0 for unused interrupts or for Paging.
        DPL	Descriptor Privilige Level, the privilege level the handler can be called from (0, 1, 2, 3).
        S	Storage Segment. Set to 0 for interrupt gates.
        D	Size of gate, (1 = 32 bits, 0 = 16 bits).
    */
    idt_descriptors[index].type_and_attr = (0x01 << 7) |               // P
                                           (0x00 << 6) | (0x00 << 5) | // DPL
                                           0xe;                        // 0b1110=0xE 32-bit interrupt gate
}

void load_idt2(uint32_t idt_address)
{
    // print loaded
    write_string_serial("Loaded IDT\n");
    struct __attribute__((packed))
    {
        uint16_t size;
        uint32_t address;
    } idt_descriptor = {sizeof(struct IDTDescriptor) * INTERRUPTS_DESCRIPTOR_COUNT - 1, idt_address};
    asm volatile("lidt %0" : : "m"(idt_descriptor));
}
void interrupt_handler2(__attribute__((unused)) struct cpu_state cpu, uint32_t interrupt, __attribute__((unused)) struct stack_state stack)
{
    write_string_serial("Keyboard interrupt\n");
    uint8_t input;
    uint8_t ascii;
    static uint32_t fe_count = 0;
    static uint32_t debug_count = 0;

    switch (interrupt)
    {
    case INTERRUPTS_KEYBOARD:
        while ((inb(0x64) & 1))
        {
            write_string_serial("Keyboard interrupt\n");
            input = keyboard_read_scan_code();

            // Debug output
            /*fb_write_cell(debug_count * 3 + 0, 'x', FB_GREEN, FB_DARK_GREY);
            fb_write_cell(debug_count * 3 + 1, "0123456789ABCDEF"[input >> 4], FB_GREEN, FB_DARK_GREY);
            fb_write_cell(debug_count * 3 + 2, "0123456789ABCDEF"[input & 0xF], FB_GREEN, FB_DARK_GREY);*/
            debug_count++;

            // Track FE codes
            if (input == 0xFE)
            {
                fe_count++;
                continue;
            }

            // Only process if it's not a break code
            if (!(input & 0x80))
            {
                if (input <= KEYBOARD_MAX_ASCII)
                {
                    ascii = keyboard_scan_code_to_ascii(input);
                    if (ascii != 0)
                    {
                        if (ascii == '\b')
                        {
                            if (BUFFER_COUNT > 0)
                            {
                                BUFFER_COUNT--;
                                // fb_write_cell(BUFFER_COUNT, ' ', FB_DARK_GREY, FB_GREEN);
                            }
                        }
                        else if (ascii == '\n')
                        {
                            BUFFER_COUNT = ((BUFFER_COUNT / 80) + 1) * 80;
                        }
                        else
                        {
                            // fb_write_cell(BUFFER_COUNT, ascii, FB_DARK_GREY, FB_GREEN);
                            BUFFER_COUNT++;
                        }
                    }
                }
            }

            buffer_index = (buffer_index + 1) % INPUT_BUFFER_SIZE;
        }

        pic_acknowledge(interrupt);
        break;

    default:
        break;
    }
}

void fake_handler(){
    write_string_serial("Fake handler\n");
    // send every single interrupt to pic acknowledge
    for (int i = 0; i < INTERRUPTS_DESCRIPTOR_COUNT; i++)
    {
        pic_acknowledge(i);
    }


    
}

void interrupts_install_idt()
{
    write_string_serial("Installing IDT\n");

    // for (int i = 0; i < INTERRUPTS_DESCRIPTOR_COUNT; i++)
    // {
    //     interrupts_init_descriptor(i, (uint32_t)fake_handler);
    // }


    // interrupts_init_descriptor(INTERRUPTS_KEYBOARD, (uint32_t)interrupt_handler_);

    // error_code_interrupt_handler_7 // 7 is for the double fault
    // error_code_interrupt_handler_8 // 8 is for the triple fault


    // interrupts_init_descriptor(INTERRUPTS_KEYBOARD, (uint32_t)interrupt_handler_2);
    

    interrupts_init_descriptor(INTERRUPTS_KEYBOARD, (uint32_t)interrupt_handler_33);
    interrupts_init_descriptor(1, (uint32_t)interrupt_handler_1);
    interrupts_init_descriptor(7, (uint32_t)interrupt_handler_7);
    interrupts_init_descriptor(8, (uint32_t)interrupt_handler_8);

    idt.address = (int32_t)&idt_descriptors;
    idt.size = sizeof(struct IDTDescriptor) * INTERRUPTS_DESCRIPTOR_COUNT;
    load_idt2((int32_t)&idt);

    /*pic_remap(PIC_PIC1_OFFSET, PIC_PIC2_OFFSET);*/
    pic_remap(PIC_1_OFFSET, PIC_2_OFFSET);

    // Unmask keyboard interrupt (IRQ1)
    outb(0x21, inb(0x21) & ~(1 << 1));

    write_string_serial("############### INSTALLED IDT ######################\n");
}

/* Interrupt handlers ********************************************************/



// 