// GCC provides these header files automatically
// They give us access to useful things like fixed-width types


#include "kernel.h"


// This is the x86's VGA textmode buffer. To display text, we write data to this memory location
volatile uint16_t *vga_buffer = (uint16_t *)0xB8000;
// By default, the VGA textmode buffer has a size of 80x25 characters
const int VGA_COLS = 80;
const int VGA_ROWS = 25;

// We start displaying text in the top-left of the screen (column = 0, row = 0)
int term_col = 0;
int term_row = 0;
uint8_t term_color = 0x0F; // Black background, White foreground



#define SERIAL_COM1_BASE 0x3F8

void setup() {
    // Set up the serial port
    serial_configure_baud_rate(SERIAL_COM1_BASE, 3);
    serial_configure_line(SERIAL_COM1_BASE);
    // Write a message to the serial port
    write_string_serial("Serial Initialised\n");

    // setup Interrupts
    interrupts_install_idt();

    // Set up the VGA text mode terminal
    terminal_initialize();
    
}


// This is our kernel's main function
extern void kernel_main()
{

    setup(); // Serial, Interrupts and Terminal setup
    

    // We're here! Let's initiate the terminal and display a message to show we got here.

    // Initiate terminal
    terminal_initialize();

    // Display some messages
    terminal_writestring("Hello, World!\n");
    terminal_setcolor(vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK));

    terminal_writestring("Welcome to the kernel.\nHold a key to activate keyboard input.\n");
    terminal_writestring("\n\n\n\nI LOVE PONIES!");

    terminal_setcolor(vga_entry_color(VGA_COLOR_MAGENTA, VGA_COLOR_BLACK));

    terminal_writestring(" YAY!!! My favourite pony is Stella\n");

    terminal_setcolor(vga_entry_color(VGA_COLOR_BLUE, VGA_COLOR_BLACK));
    terminal_writestring("She's SUCH a cutie!\n\n");
    
    terminal_setcolor(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BROWN));
    terminal_writestring("Luna's OS: ");


    while (1==1){
        asm volatile("hlt"); // wait for the next interrupt
        // int big = 0;
        // char c;
        // getc(&c);
        // char* s = "Key Pressed: ";
        // s[12] = c;
        // // terminal_putc(c);
        // terminal_writestring(". >:)");
        // // write_string_serial("Key Pressed: ");
        // write_serial(c);
        // write_serial('.');
        // while (big < 10000000){
        //     big++;
        // }
    }
    
    
}

