#ifndef SERIAL_H
#define SERIAL_H

#include "../prov_drivers/io.h"
#include "serial.h"
#include "common.h"
#include <stdint.h>

void serial_configure_baud_rate(unsigned short com, unsigned short divisor);
void serial_configure_line(unsigned short com);
int serial_is_transmit_fifo_empty(unsigned int com);
void write_serial(char c);
void write_string_serial(const char *s);
void write_serialtest();
extern void write_serial_num(uint32_t num);
#endif // SERIAL_H
