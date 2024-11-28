
#ifndef INCLUDE_KEYBOARD_H
#define INCLUDE_KEYBOARD_H
#define KEYBOARD_MAX_ASCII 83
#include <stdint.h>

#include "io.h"
#include "pic.h"


extern uint8_t keyboard_read_scan_code(void);
extern uint8_t keyboard_scan_code_to_ascii(uint8_t);
#endif /* INCLUDE_KEYBOARD_H */
