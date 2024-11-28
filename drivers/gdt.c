
#include "gdt.h"


typedef struct
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed)) gdt_entry_t;

// Structure for GDT pointer
typedef struct
{
    uint16_t limit; // Size of the GDT (in bytes) - 1
    uint32_t base;  // Base address of the GDT
} __attribute__((packed)) gdt_ptr_t;

// Function to create a null descriptor
gdt_entry_t create_null_descriptor()
{
    gdt_entry_t entry = {0, 0, 0, 0, 0, 0};
    return entry;
}

// Function to create a code segment descriptor
gdt_entry_t create_code_segment()
{
    gdt_entry_t entry;
    entry.limit_low = 0xFFFF;
    entry.base_low = 0x00;
    entry.base_middle = 0x00;
    entry.access = 0x9A;      // Code segment (present, executable, readable)
    entry.granularity = 0xCF; // 32-bit size, 4 KB granularity
    entry.base_high = 0x00;
    return entry;
}

// Function to create a data segment descriptor
gdt_entry_t create_data_segment()
{
    gdt_entry_t entry;
    entry.limit_low = 0xFFFF;
    entry.base_low = 0x00;
    entry.base_middle = 0x00;
    entry.access = 0x92;      // Data segment (present, writable, not executable)
    entry.granularity = 0xCF; // 32-bit size, 4 KB granularity
    entry.base_high = 0x00;
    return entry;
}

// GDT pointer structure
gdt_ptr_t gdt_ptr;

// GDT entries (Null, Code, Data)
gdt_entry_t gdt_entries[3];

// Function to load the GDT using lgdt
void gdt_flush(uint32_t gdt_ptr)
{
    // loaded GDT
    write_string_serial("GDT loaded and flushed!");
    asm volatile(
        "lgdt (%0)"    // Load the GDT using the pointer passed to the function
        :              // No output operands
        : "r"(gdt_ptr) // Input operand: gdt_ptr is passed as a pointer to the GDT
    );
}

// Function to initialize the GDT
void init_gdt()
{
    gdt_ptr.limit = sizeof(gdt_entry_t) * 3 - 1; // Size of the GDT (3 entries)
    gdt_ptr.base = (uint32_t)&gdt_entries;       // Address of the GDT entries

    // Initialize GDT entries
    gdt_entries[0] = create_null_descriptor();
    gdt_entries[1] = create_code_segment();
    gdt_entries[2] = create_data_segment();

    // Load the GDT
    gdt_flush((uint32_t)&gdt_ptr);
}