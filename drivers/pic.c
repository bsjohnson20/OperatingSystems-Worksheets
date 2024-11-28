
#include "io.h"
#include <stdint.h>

#include "pic.h"

#define PIC1_PORT_A 0x20
#define PIC2_PORT_A 0xA0
/* The PIC interrupts have been remapped */
#define PIC1_START_INTERRUPT 0x20
#define PIC2_START_INTERRUPT 0x28
#define PIC2_END_INTERRUPT PIC2_START_INTERRUPT + 7
#define PIC_ACK 0x20
/** pic_acknowledge:
 * Acknowledges an interrupt from either PIC 1 or PIC 2.
 *
 * @param num The number of the interrupt
 */

// PIC RemAP

#include "serial.h"

void pic_remap(uint32_t offset1, uint32_t offset2)
{

    // write offsets to serial
    write_string_serial("PIC Remap\n");
    write_string_serial("Offset1: ");
    write_serial_num(offset1);
    write_string_serial("\n");
    write_string_serial("Offset2: ");
    write_serial_num(offset2);
    write_string_serial("\n");

    outb(PIC_1_COMMAND, PIC_ICW1_INIT + PIC_ICW1_ICW4); // starts the initialization sequence (in cascade mode)
    outb(PIC_2_COMMAND, PIC_ICW1_INIT + PIC_ICW1_ICW4);
    outb(PIC_1_DATA, offset1); // ICW2: Master PIC vector offset
    outb(PIC_2_DATA, offset2); // ICW2: Slave PIC vector offset
    outb(PIC_1_DATA, 4);       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    outb(PIC_2_DATA, 2);       // ICW3: tell Slave PIC its cascade identity (0000 0010)

    outb(PIC_1_DATA, PIC_ICW4_8086);
    outb(PIC_2_DATA, PIC_ICW4_8086);

    // Setup Interrupt Mask Register (IMR)
    outb(PIC_1_DATA, 0xFD); // 1111 1101 - Enable IRQ 1 only (keyboard).
    outb(PIC_2_DATA, 0xFF);

    asm("sti"); // Enable interrupts.
}


extern void pic_acknowledge(unsigned int interrupt)
{
    write_string_serial("PIC ACK\n");

    if (interrupt < PIC1_START_INTERRUPT || interrupt > PIC2_END_INTERRUPT)
    {
        return;
    }
    if (interrupt < PIC2_START_INTERRUPT)
    {
        outb(PIC1_PORT_A, PIC_ACK);
    }
    else
    {
        outb(PIC2_PORT_A, PIC_ACK);
    }
}
