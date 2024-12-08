#ifndef INCLUDE_KEYBOARD_H
#define INCLUDE_KEYBOARD_H

#pragma once

#define KEYBOARD_MAX_ASCII 83 

#include "type.h"

u8int keyboard_read_scan_code(void);

u8int keyboard_scan_code_to_ascii(u8int);

// static char* keyboard_buffer[80*256];
// int keyboard_buffer_index = 0;
// static uint8_t keyboard_buffer_index = 0;
void getc(char* c);

void add_to_keyboard_buffer(char c);
void remove_from_keyboard_buffer();
int get_keyboard_buffer_index();
char* get_keyboard_buffer();
#endif /* INCLUDE_KEYBOARD_H */

