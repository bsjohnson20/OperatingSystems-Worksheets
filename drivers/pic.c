
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


    outb(PIC1_PORT_A, offset1);
    outb(PIC2_PORT_A, offset2);
    outb(PIC1_PORT_A, 4);
    outb(PIC2_PORT_A, 2);
    outb(PIC1_PORT_A, 1);
    outb(PIC2_PORT_A, 1);
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
