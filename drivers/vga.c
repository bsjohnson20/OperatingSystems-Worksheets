



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





// void reset_keyboard_buffer(){
//     for (int i = 0; i < 80; i++)
//     {
//         *keyboard_buffer[i] = 0;
//     }
//     keyboard_buffer_index = 0;
    
// }

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
    // update hardware cursor
    fb_move_cursor(0);

    // update buffer
    reset_keyboard_buffer();
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
    // clear buffer
    reset_keyboard_buffer();
}

void terminal_backspace(){
    if (terminal_column > 0)
    {
        terminal_column--;
        terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);

        // delete from keyboard buffer
        // keyboard_buffer_index--;
        reset_keyboard_buffer();
    }
    if (terminal_column == 0 && terminal_row > 0)
    {
        terminal_row--;
        terminal_column = VGA_WIDTH - 1;
        // delete from keyboard buffer
        // keyboard_buffer_index--;
    }
    fb_move_cursor(terminal_row * VGA_WIDTH + terminal_column);
}

void terminal_delete_last_line()
{
    // delete this line if col greater than 0
    if (terminal_column > 0)
    {
        terminal_column = VGA_WIDTH;
    }
    else
    {
        terminal_row--;
        terminal_column = VGA_WIDTH;
    }
    while (terminal_column > 0)
    {
        terminal_column--;
        terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
        fb_move_cursor(terminal_row * VGA_WIDTH + terminal_column);
    } 
    fb_move_cursor(terminal_row * VGA_WIDTH + terminal_column);
}



extern void terminal_write(const char *data, size_t size)
{
    for (size_t i = 0; i < size; i++){
        terminal_putchar(data[i]);
        // *keyboard_buffer[keyboard_buffer_index] = data[i];
        // keyboard_buffer_index++;
    }
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



extern void terminal_putchar(char c)
{

    // What happens if we get past the last row? We need to scroll the terminal
    if (terminal_row >= VGA_HEIGHT)
    {
        terminal_scroll(1);
        terminal_row = VGA_HEIGHT - 1;
        // update the hardware cursor
       
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
        reset_keyboard_buffer();

        break;
    }



    default: // Normal characters just get displayed and then increment the column
    {
        if (terminal_column >= VGA_WIDTH)
        {
            terminal_column = 0;
            terminal_row++;
        }

        const size_t index = (VGA_WIDTH * terminal_row) + terminal_column; // Like before, calculate the buffer index
        terminal_buffer[index] = vga_entry(c, terminal_color);
        terminal_column++;

        // add to keyboard buffer
        // *keyboard_buffer[keyboard_buffer_index] = c;
        // keyboard_buffer_index++;


        // increment hardware cursor using io
        
        break;
    }
    }

    // // What happens if we get past the last column? We need to reset the column to 0, and increment the row to get to a new line
    // if (terminal_column >= VGA_WIDTH)
    // {
    //     terminal_column = 0;
    //     terminal_row++;
    // }

    

    fb_move_cursor(terminal_row * VGA_WIDTH + terminal_column);

    // test for cursor pos
    // write_serial_num(terminal_row * VGA_WIDTH + terminal_column);
    // terminal_writeint(terminal_row * VGA_WIDTH + terminal_column);
}

void asm_display_num(int num){
    char str[20];
    convert_num_to_string(num, str);
    terminal_writestring(str);
}



// legacy
char *fb = (char *) 0x000B8000;
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
    {
        i++;
        fg++;
        bg++;
        terminal_putchar(c);
        // fb[i] = c;
        // fb[i + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
    }