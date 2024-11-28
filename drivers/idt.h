// IDT

#ifndef IDT_H
#define IDT_H



#include "interrupts.h"
#include "pic.h"
#include "io.h"

#include <stdint.h>


#define IDT_DESCRIPTOR_COUNT 256
#define IDT_KEYBOARD 33
#define INPUT_BUFFER_SIZE 256
// uint8_t input_buffer[INPUT_BUFFER_SIZE];
// uint8_t buffer_index = 0;
// struct IDTDescriptor idt_descriptors[IDT_DESCRIPTOR_COUNT];
// struct IDT idt;
// u32int BUFFER_COUNT;

//  void init_pics(int pic1, int pic2);
extern void init_pics();

#endif // IDT_H

