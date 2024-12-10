#include "interrupts.h"
#include "pic.h"
#include "io.h"
#include "../drivers/vga.h"
#include "keyboard.h"
#include "../drivers/serial.h"


#define KEYBOARD_BUFFER_SIZE 256

#define INTERRUPTS_DESCRIPTOR_COUNT 256 
#define INTERRUPTS_KEYBOARD 33 
#define INPUT_BUFFER_SIZE 256

u8int input_buffer[INPUT_BUFFER_SIZE];
u8int buffer_index = 0;
 
struct IDTDescriptor idt_descriptors[INTERRUPTS_DESCRIPTOR_COUNT];
struct IDT idt;

u32int BUFFER_COUNT;

void interrupts_init_descriptor(s32int index, u32int address)
{
	idt_descriptors[index].offset_high = (address >> 16) & 0xFFFF; // offset bits 0..15
	idt_descriptors[index].offset_low = (address & 0xFFFF); // offset bits 16..31

	idt_descriptors[index].segment_selector = 0x08; // The second (code) segment selector in GDT: one segment is 64b.
	idt_descriptors[index].reserved = 0x00; // Reserved.

	/*
	   Bit:     | 31              16 | 15 | 14 13 | 12 | 11     10 9 8   | 7 6 5 | 4 3 2 1 0 |
	   Content: | offset high        | P  | DPL   | S  | D and  GateType | 0 0 0 | reserved
		P	If the handler is present in memory or not (1 = present, 0 = not present). Set to 0 for unused interrupts or for Paging.
		DPL	Descriptor Privilige Level, the privilege level the handler can be called from (0, 1, 2, 3).
		S	Storage Segment. Set to 0 for interrupt gates.
		D	Size of gate, (1 = 32 bits, 0 = 16 bits).
	*/
	idt_descriptors[index].type_and_attr =	(0x01 << 7) |			// P
						(0x00 << 6) | (0x00 << 5) |	// DPL
						0xe;				// 0b1110=0xE 32-bit interrupt gate
}

void interrupts_install_idt()
{
	
	interrupts_init_descriptor(INTERRUPTS_KEYBOARD, (u32int) interrupt_handler_33);


	idt.address = (s32int) &idt_descriptors;
	idt.size = sizeof(struct IDTDescriptor) * INTERRUPTS_DESCRIPTOR_COUNT;
	load_idt((s32int) &idt);

	/*pic_remap(PIC_PIC1_OFFSET, PIC_PIC2_OFFSET);*/
	pic_remap(PIC_1_OFFSET, PIC_2_OFFSET);

    // Unmask keyboard interrupt (IRQ1)
    outb(0x21, inb(0x21) & ~(1 << 1));
}


// create a buffer to store text input on the line
char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
int keyboard_buffer_index = 0;

void reset_keyboard_buffer(){
    for (int i = 0; i < KEYBOARD_BUFFER_SIZE; i++)
    {
        keyboard_buffer[i] = 0;
    }
    buffer_index = 0;
}

void serial_keyboard_buffer(){
    for (int i = 0; i < KEYBOARD_BUFFER_SIZE; i++)
    {
        write_serial(keyboard_buffer[i]);
    }
    write_serial('\n');
}

void add_to_keyboard_buffer(char c){
    if (keyboard_buffer_index >= KEYBOARD_BUFFER_SIZE)
    {
        return;
    }
    keyboard_buffer[keyboard_buffer_index] = c;
    keyboard_buffer_index++;
}

void remove_from_keyboard_buffer(){
    keyboard_buffer_index--;
    keyboard_buffer[keyboard_buffer_index] = 0;
}

int get_keyboard_buffer_index(){
    return keyboard_buffer_index;
}

void hlt(){
    asm volatile("hlt");
}

void getc(char* c){

    int current_index = get_keyboard_buffer_index();
    

    // hlt();

    if (keyboard_buffer_index <= 0) {
        return;
    }

    char x = keyboard_buffer[keyboard_buffer_index];
    keyboard_buffer[keyboard_buffer_index] = ' ';
    keyboard_buffer_index--;
    
    write_serial('[');
    write_serial_num(keyboard_buffer_index);    
    write_serial(']');

    *c = 'c';
    
}

bool readc = false;

/* Interrupt handlers ********************************************************/

void interrupt_handler(__attribute__((unused)) struct cpu_state cpu, u32int interrupt, __attribute__((unused)) struct stack_state stack) {
    u8int input;
    u8int ascii;
    static u32int fe_count = 0;
    // static u32int debug_count = 0;
    
    

    switch (interrupt) {
        
        case INTERRUPTS_KEYBOARD:
            terminal_setcolor(VGA_COLOR_BLACK);
            while ((inb(0x64) & 1)) {  
                input = keyboard_read_scan_code();

                // write_string_serial("Interrupt: ");
                // write_serial(input);
                
                // Debug output
                // fb_write_cell(debug_count * 3 + 0, 'x', VGA_COLOR_GREEN, VGA_COLOR_DARK_GREY);
                // fb_write_cell(debug_count * 3 + 1, "0123456789ABCDEF"[input >> 4], VGA_COLOR_GREEN, VGA_COLOR_DARK_GREY);
                // fb_write_cell(debug_count * 3 + 2, "0123456789ABCDEF"[input & 0xF], VGA_COLOR_GREEN, VGA_COLOR_DARK_GREY);
                // debug_count++;

                // Track FE codes
                if (input == 0xFE) {
                    fe_count++;
                    continue;
                }
                
                // Only process if it's not a break code
                if (!(input & 0x80)) {
                    if (input <= KEYBOARD_MAX_ASCII) {
                        ascii = keyboard_scan_code_to_ascii(input);
                        if (ascii != 0) {
                            if (ascii == '\b') {
                                terminal_column--;
                                terminal_backspace();
                                remove_from_keyboard_buffer();
                            }
                            else if (ascii == 'd'){
                                terminal_delete_last_line();
                                reset_keyboard_buffer();
                            }
                            else if (ascii == '\n') {
                                reset_keyboard_buffer();
                                terminal_putc('\n'); // BUFFER_COUNT = ((BUFFER_COUNT / 80) + 1) * 80;
                            }
                            else {
                                terminal_putc(ascii);
                                keyboard_buffer[buffer_index] = ascii;

                                serial_keyboard_buffer();
                                
                                // fb_write_cell(BUFFER_COUNT, ascii, VGA_COLOR_DARK_GREY, VGA_COLOR_GREEN);
                                // BUFFER_COUNT++;
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
