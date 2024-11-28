// GCC provides these header files automatically
// They give us access to useful things like fixed-width types
#include <stddef.h>
#include <stdint.h>

#include "serial.h"
#include "vga.h"
#include "common.h"
#include <stdint.h>

// #include "kernel.h"

// First, let's do some basic checks to make sure we are using our x86-elf cross-compiler correctly
// #if defined(__linux__)
// #error "This code must be compiled with a cross-compiler"
// #elif !defined(__i386__)
// #error "This code must be compiled with an x86-elf compiler"
// #endif

// This is the x86's VGA textmode buffer. To display text, we write data to this memory location
volatile uint16_t *vga_buffer = (uint16_t *)0xB8000;
// By default, the VGA textmode buffer has a size of 80x25 characters
const int VGA_COLS = 80;
const int VGA_ROWS = 25;

// We start displaying text in the top-left of the screen (column = 0, row = 0)
int term_col = 0;
int term_row = 0;
uint8_t term_color = 0x0F; // Black background, White foreground




// This is our kernel's main function
void kernel_main()
{
    // We're here! Let's initiate the terminal and display a message to show we got here.

    // Initiate terminal
    terminal_initialize();

    // Display some messages
    terminal_writestring("Hello, World!\n");
    terminal_setcolor(vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK));

    terminal_writestring("Welcome to the kernel.\n");
    terminal_writestring("\n\n\n\nI LOVE PONIES!");

    terminal_setcolor(vga_entry_color(VGA_COLOR_MAGENTA, VGA_COLOR_BLACK));

    terminal_writestring(" YAY!!! My favourite pony is Stella\n");

    terminal_setcolor(vga_entry_color(VGA_COLOR_BLUE, VGA_COLOR_BLACK));
    terminal_writestring("She's SUCH a cutie!\n\n");
    
    terminal_setcolor(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BROWN));
    terminal_writestring("TERMINAL INPUT (never works): ");
    
}

