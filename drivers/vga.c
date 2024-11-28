#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "io.h"



#include "vga.h"

/* The I/O ports */
#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT 0x3D5
/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND 15

extern void fb_move_cursor(unsigned short pos)
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT, ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT, pos & 0x00FF);
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t *const VGA_MEMORY = (uint16_t *)0xB8000;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t *terminal_buffer;

extern void terminal_initialize(void)
{
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = VGA_MEMORY;
    for (size_t y = 0; y < VGA_HEIGHT; y++)
    {
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
    fb_move_cursor(0);
}

extern void terminal_setcolor(uint8_t color)
{
    terminal_color = color;
}

extern void terminal_putentryat(unsigned char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

extern void terminal_scroll(int line)
{
    uint16_t *src = VGA_MEMORY + line * VGA_WIDTH;
    uint16_t *dst = VGA_MEMORY;

    for (size_t y = 0; y < VGA_HEIGHT - line; y++)
    {
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            dst[y * VGA_WIDTH + x] = src[y * VGA_WIDTH + x];
        }
    }

    for (size_t y = VGA_HEIGHT - line; y < VGA_HEIGHT; y++)
    {
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            dst[y * VGA_WIDTH + x] = vga_entry(' ', terminal_color);
        }
    }
}

extern void terminal_backspace(){
    if (terminal_column > 0){
        terminal_column--;
        terminal_putchar(' ');
    }
    else if (terminal_row > 0){
        terminal_row--;
        terminal_column = VGA_WIDTH - 1;
        terminal_putchar(' ');
        terminal_column = VGA_WIDTH - 1;
    }
}

extern void terminal_delete_last_line()
{
    size_t x;
    uint16_t *ptr;

    for (x = 0; x < VGA_WIDTH; x++)
    {
        ptr = VGA_MEMORY + (VGA_WIDTH * (VGA_HEIGHT - 1)) + x;
        *ptr = vga_entry(' ', terminal_color);
    }
}

extern void terminal_putchar(char c)
{

    // What happens if we get past the last row? We need to scroll the terminal
    if (terminal_row >= VGA_HEIGHT)
    {
        terminal_scroll(1);
        terminal_row = VGA_HEIGHT - 1;
        // update the hardware cursor
        fb_move_cursor(terminal_row * VGA_WIDTH + terminal_column);
    }

    // if (++terminal_column == VGA_WIDTH)
    // {
    //     terminal_column = 0;
    //     if (++terminal_row == VGA_HEIGHT)
    //     {
    //         terminal_scroll(1);
    //         terminal_row = VGA_HEIGHT - 1;
    //     }
    // }

    switch (c)
    {
    case '\n': // Newline characters should return the column to 0, and increment the row
    {
        terminal_column = 0;
        terminal_row++;

        // increment hardware cursor using io to new line
        fb_move_cursor(terminal_row * VGA_WIDTH + terminal_column);

        break;
    }

    default: // Normal characters just get displayed and then increment the column
    {
        const size_t index = (VGA_WIDTH * terminal_row) + terminal_column; // Like before, calculate the buffer index
        terminal_buffer[index] = vga_entry(c, terminal_color);
        terminal_column++;

        // increment hardware cursor using io
        fb_move_cursor(terminal_row * VGA_WIDTH + terminal_column);
        break;
    }
    }

    // // What happens if we get past the last column? We need to reset the column to 0, and increment the row to get to a new line
    // if (terminal_column >= VGA_WIDTH)
    // {
    //     terminal_column = 0;
    //     terminal_row++;
    // }

    

    
}

extern void terminal_write(const char *data, size_t size)
{
    for (size_t i = 0; i < size; i++)
        terminal_putchar(data[i]);
}

extern void terminal_writestring(const char *data)
{
    terminal_write(data, strlen(data));
}

extern void terminal_writeint(int data)
{
    char str[20];
    convert_num_to_string(data, str);
    
    terminal_writestring(str);
}