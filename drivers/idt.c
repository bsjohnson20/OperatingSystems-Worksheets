

#include "idt.h"
#include "io.h"


#include "interrupts.h"
#include "pic.h"

#define PIC1 0x20 /* IO base address for master PIC */
#define PIC2 0xA0 /* IO base address for slave PIC */
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2 + 1)

typedef struct idt_entry_t
{
    unsigned short base_lo;
    unsigned short sel;
    unsigned char always0;
    unsigned char flags;
    unsigned short base_hi;
} __attribute__((packed)) idt_entry_t;

extern void idt_flush(uint32_t address);

idt_entry_t idt_entries[256];

#define PIC1 0x20
#define PIC2 0xA0

#define ICW1 0x11
#define ICW4 0x01

/* init_pics()
 * init the PICs and remap them
 */
 void init_pics()
{
    // Command ports for PICs
    const uint16_t MASTER_CMD_PORT = 0x20;
    const uint16_t MASTER_DATA_PORT = 0x21;
    const uint16_t SLAVE_CMD_PORT = 0xA0;
    const uint16_t SLAVE_DATA_PORT = 0xA1;

    // Start initialization of both PICs
    outb(MASTER_CMD_PORT, 0x11); // Start initialization sequence on master PIC
    outb(SLAVE_CMD_PORT, 0x11);  // Start initialization sequence on slave PIC

    // Set vector offsets for the PICs (usually 0x20 for master, 0x28 for slave)
    outb(MASTER_DATA_PORT, 0x20); // Set the master PIC vector offset
    outb(SLAVE_DATA_PORT, 0x28);  // Set the slave PIC vector offset

    // Configure master PIC to communicate with slave PIC at IRQ2 (bit 2 of 0x21)
    outb(MASTER_DATA_PORT, 0x04); // Tell the master PIC there is a slave on IRQ2
    outb(SLAVE_DATA_PORT, 0x02);  // Tell the slave PIC its identity (IRQ2)

    // Set the PICs to 8086/88 mode (required for compatibility)
    outb(MASTER_DATA_PORT, 0x01); // Set 8086/88 mode on master PIC
    outb(SLAVE_DATA_PORT, 0x01);  // Set 8086/88 mode on slave PIC

    // Mask all interrupts (disable all IRQs temporarily)
    outb(MASTER_DATA_PORT, 0xFF); // Mask all IRQs on the master PIC
    outb(SLAVE_DATA_PORT, 0xFF);  // Mask all IRQs on the slave PIC
}


