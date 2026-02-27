
// #include "vga.h"


#include "serial.h"

/* The I/O ports */
/* All the I/O ports are calculated relative to the data port. This is because
 * all serial ports (COM1, COM2, COM3, COM4) have their ports in the same
 * order, but they start at different values.
 */
#define SERIAL_COM1_BASE 0x3F8 /* COM1 base port */
#define SERIAL_DATA_PORT(base) (base)
#define SERIAL_FIFO_COMMAND_PORT(base) (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base) (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base) (base + 5)
/* The I/O port commands */
/* SERIAL_LINE_ENABLE_DLAB:
 * Tells the serial port to expect first the highest 8 bits on the data port,
 * then the lowest 8 bits will follow
 */
#define SERIAL_LINE_ENABLE_DLAB 0x80
/** serial_configure_baud_rate:
* Sets the speed of the data being sent. The default speed of a serial
* port is 115200 bits/s. The argument is a divisor of that number, hence
* the resulting speed becomes (115200 / divisor) bits/s.
27
*
* @param com The COM port to configure
* @param divisor The divisor
*/
extern void serial_configure_baud_rate(unsigned short com, unsigned short divisor)
{
    outb(SERIAL_LINE_COMMAND_PORT(com),
         SERIAL_LINE_ENABLE_DLAB);
    outb(SERIAL_DATA_PORT(com),
         (divisor >> 8) & 0x00FF);
    outb(SERIAL_DATA_PORT(com),
         divisor & 0x00FF);
}

/** serial_configure_line:
 * Configures the line of the given serial port. The port is set to have a
 * data length of 8 bits, no parity bits, one stop bit and break control
 * disabled.
 *
 * @param com The serial port to configure
 */
void serial_configure_line(unsigned short com)
{
    /* Bit: | 7 | 6 | 5 4 3 | 2 | 1 0 |
     * Content: | d | b | prty | s | dl |
     * Value: | 0 | 0 | 0 0 0 | 0 | 1 1 | = 0x03
     */
    outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

/** serial_is_transmit_fifo_empty:
 * Checks whether the transmit FIFO queue is empty or not for the given COM
 * port.
 *
 * @param com The COM port
 * @return 0 if the transmit FIFO queue is not empty
 * 1 if the transmit FIFO queue is empty
 */
int serial_is_transmit_fifo_empty(unsigned int com)
{
    /* 0x20 = 0010 0000 */
    return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}

void write_serial(char c)
{
    while (!serial_is_transmit_fifo_empty(SERIAL_COM1_BASE))
        ;
    outb(SERIAL_DATA_PORT(SERIAL_COM1_BASE), c);
}

extern void write_string_serial(const char *s)
{
    while (*s)
        write_serial(*s++);
}

/**
 * register_toSerial:
 * Writes the content of the register to the serial port. The format is
 * [content].
 *
 * @param reg The register to write to the serial port
 */
extern void register_toSerial(int reg)
{
    // write a register to the serial port
    char buf[10];
    convert_num_to_string(reg, buf);

    write_serial('[');
    write_string_serial(buf);
    write_serial(']');
}

extern void write_serialtest()
{
    const char *buf = "I love ponies!\nYAY!\nMy favourite pony is Stella\nA custom oc!";
    //     const char *buf = "Lorem ipsum odor amet, consectetuer adipiscing elit. Fusce orci dictumst ut nisi ridiculus pharetra porta pulvinar nullam feugiat libero leo rutrum imperdiet ad finibus ut convallis nibh ullamcorper sagittis, ut tortor curabitur mollis vestibulum varius condimentum enim bibendum vel natoque aliquam integer sit sociosqu netus interdum at cubilia sodales tempus nisi gravida mus et pulvinar aliquet quis egestas porttitor scelerisque quam litora litora nibh metus augue sit diam mi mus lectus nibh senectus fusce tortor lacus.";
    write_string_serial(buf);
}

// write_num
extern void write_serial_num(uint32_t num)
{
    // write a number to the serial port
    char buf[32];
    convert_num_to_string(num, buf);
    write_string_serial(buf);
}