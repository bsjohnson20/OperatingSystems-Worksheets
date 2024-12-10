; We declare the 'kernel_main' label as being external to this file
; That's because it's the name of the main C function in 'kernelc'

 
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
section .multiboot
    align 4
	dd MAGIC
	dd MBFLAGS
	dd CHECKSUM
 
; define the kernel stack size
KERNEL_STACK_SIZE equ 8192 ; 8KB


extern kernel_main



; This section contains data initialised to zeroes when the kernel is loaded
section .bss
	; Our C code will need a stack to run Here, we allocate 4096 bytes (or 4 Kilobytes) for our stack
	; We can expand this later if we want a larger stack For now, it will be perfectly adequate
	align 16
	stack_bottom:
		resb KERNEL_STACK_SIZE ; reserve 8192 bytes for the stack ; Reserve a 4096-byte (4K) stack
	stack_top:
 
; This section contains our actual assembly code to be run when our kernel loads
section .text
	; Here is the 'start' label we mentioned before This is the first code that gets run in our kernel
	start:
		; First thing's first: we want to set up an environment that's ready to run C code
		; C is very relaxed in its requirements: All we need to do is to set up the stack
		; Please note that on x86, the stack grows DOWNWARD This is why we start at the top
		mov esp, stack_top ; Set the stack pointer to the top of the stack


		; Worksheet 2: pt1 Task 2
		extern sum_of_three;
		extern mult_of_three;
		extern sub_num;

		extern terminal_initialize
		extern terminal_putc
		call terminal_initialize

		push 1
		push 2
		push 3
		call sum_of_three

		extern asm_display_num
		push eax
		call asm_display_num
		push '\\'
		call terminal_putc

		push 1
		push 2
		push 4
		call mult_of_three
		push eax
		call asm_display_num
		push '\\'
		call terminal_putc

		push 1
		push 2
		call sub_num
		push eax
		call asm_display_num
		push '\\'
		call terminal_putc

		; gives time to show the output
		mov eax, 120
		sleep2:
			mov ecx, 499999
			sleep:
				loop sleep
			sub eax, 1
			CMP eax, 0
			JNE sleep2





		; Now that we have a stack, we can call our C function
		call kernel_main ; Call the 'kernel_main' function in 'kernelc.c'

		; lp:
		; 	jmp $
        
