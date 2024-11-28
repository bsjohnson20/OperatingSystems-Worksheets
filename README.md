# Worksheet 1 - Arrays and I/O

### Purpose

- To emulate and run a BareBones OS using assembly and C
- Using hardware addresses to interact with the system
- Writing to the screen

### Things learnt

- Basic API means extensions are a lot easier

```c

// This code resets the position variables, the cursor and overwrites the buffer with spaces to clear the screen
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
}
```
- Hardware addresses

```nasm
  fb  equ 0xb8000 ; frame buffer address
```
Sending data to that address prints to the screen
Using ascii codes and some bits to denote the colour. 
It's a 16 bit type. 8 bits for the character, 4 for the background, and 4 for the foreground.


- Changing the cursor

```c
// uses a assembly file wrapped in a c function to move the cursor
void fb_move_cursor(unsigned short pos)
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT, ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT, pos & 0x00FF);
}
```

- Serial communication

```c
// Uses the io.h library to write to the serial port
// Test function
extern "C" void write_serialtest()
{
    const char *buf = "I love ponies!\nYAY!\nMy favourite pony is Stella\nA custom oc!";
    write_string_serial(buf);
}
```

- Externs

```c
// externs to allow other files to use the functions within another
extern "C" void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);
```


## Run Locally

- Clone the project

```bash
git clone https://gitlab.uwe.ac.uk/
```

- cd into the directory

```bash
cd OS-worksheet1
```

### Compile tasks

Compile OS

```bash
make os.iso
```

**Telnet** 
```bash
make telnet
```

**Serial**

```bash
make serial
```

---


## Outputs

### Print to screen


## Contributing

Contributions are always welcome!

See `contributing.md` for ways to get started.

Please adhere to this project's `code of conduct`.

## License

[MIT](https://choosealicense.com/licenses/mit/)
