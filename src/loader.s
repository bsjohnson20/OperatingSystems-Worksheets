; We declare the 'kernel_main' label as being external to this file
; That's because it's the name of the main C function in 'kernelc'
extern kernel_main
 
; We declare the 'start' label as global (accessible from outside this file), since the linker will need to know where it is
; In a bit, we'll actually take a look at the code that defines this label
global start
 
; Our bootloader, GRUB, needs to know some basic information about our kernel before it can boot it
; We give GRUB this information using a standard known as 'Multiboot'
; To define a valid 'Multiboot header' that will be recognised by GRUB, we need to hard code some
; constants into the executable The following code calculates those constants
MBALIGN  equ  1 << 0            ; align loaded modules on page boundaries
MEMINFO  equ  1 << 1            ; provide memory map
MBFLAGS  equ  MBALIGN | MEMINFO ; this is the Multiboot 'flag' field
MAGIC    equ  0x1BADB002        ; 'magic number' lets bootloader find the header
CHECKSUM equ -(MAGIC + MBFLAGS)   ; checksum of above, to prove we are multiboot

; We now start the section of the executable that will contain our Multiboot header
section multiboot
    align 4
	dd MAGIC
	dd MBFLAGS
	dd CHECKSUM
 
; define the kernel stack size
KERNEL_STACK_SIZE equ 8192 ; 8KB


; EXTERNAL FUNCTIONS
extern init_gdt
extern interrupts_install_idt
extern init_pics
extern write_serial_num



; This section contains data initialised to zeroes when the kernel is loaded
section bss
	; Our C code will need a stack to run Here, we allocate 4096 bytes (or 4 Kilobytes) for our stack
	; We can expand this later if we want a larger stack For now, it will be perfectly adequate
	align 16
	stack_bottom:
		resb KERNEL_STACK_SIZE ; reserve 8192 bytes for the stack ; Reserve a 4096-byte (4K) stack
	stack_top:
 
; This section contains our actual assembly code to be run when our kernel loads
section text
	; Here is the 'start' label we mentioned before This is the first code that gets run in our kernel
	start:
		; First thing's first: we want to set up an environment that's ready to run C code
		; C is very relaxed in its requirements: All we need to do is to set up the stack
		; Please note that on x86, the stack grows DOWNWARD This is why we start at the top
		mov esp, stack_top ; Set the stack pointer to the top of the stack


        ; setup GDT
        call init_gdt


		

		call init_pics ; initialize the pics

		; setup IDT
		call interrupts_install_idt

		; enable interrupts
		sti

		; jmp to protected mode
		jmp 0x08:set_cs
		
		set_cs:
			mov ax, 0x10
			mov ds, ax
			mov es, ax
			mov fs, ax
			mov gs, ax
			mov ss, ax
		


		main:
 
			extern make_square
			extern make_triangle
			extern make_circle
			extern make_diamond

			extern terminal_initialize
			call terminal_initialize

			call make_square
			call make_triangle
			call make_circle
			call make_diamond



		; Now we have a C-worthy (haha!) environment ready to run the rest of our kernel
		; At this point, we can call our main C function
		call kernel_main
 
		; loop 10000 times. Only then add one to eax, send to serial
	l2:
		mov ecx, 100000000
		count:
			loop count
			add eax, 1
			push eax
			call write_serial_num
			
			pop eax
			jmp l2

		; ; If, by some mysterious circumstances, the kernel's C code ever returns, all we want to do is to hang the CPU
		; hang:
		; 	; cli      ; Disable CPU interrupts
		; 	hlt      ; Halt the CPU
		; 	jmp hang ; If that didn't work, loop around and try again
		; ; Love you <3