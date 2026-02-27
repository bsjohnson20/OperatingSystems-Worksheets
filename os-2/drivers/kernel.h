#ifndef KERNEL_H
#define KERNEL_H

#include <stddef.h>
#include <stdint.h>

#include "serial.h"
#include "vga.h"
#include "common.h"

#include "../prov_drivers/keyboard.h"
#include "../prov_drivers/interrupts.h"

extern void kernel_main();

#endif
