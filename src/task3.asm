%include "asm_io.inc"
segment .data
    msg1 db "Enter a number: ", 0
    msg2 db "Hello world!", 0
    msg3 db "x is not within 50-100!"


segment .bss
    input resb 100

segment .text
    global asm_main
asm_main:
    enter 0,0
    pusha

    mov eax, msg1
    call print_string
    mov eax, input
    call read_int
    mov ecx, eax
repeat:
    ; the loop instruction decrements ecx and jumps to the target
    ; if ecx is not zero. This is a way to repeat a block of code
    ; a certain number of times.
    mov eax, msg2
    call print_string
    mov eax, input
    call print_string
    loop repeat

    popa
    mov eax, 0
    leave
    ret
