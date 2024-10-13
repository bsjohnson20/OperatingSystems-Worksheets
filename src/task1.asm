%include "/home/bradley3.johnson/os-root/os-worksheet-1/src/asm_io.inc"

; take a number and another from user and add and return to user.

segment .data
integer1 dd 15 ; first int
integer2 dd 6 ; second int
segment .bss
result resd 1 ; result
segment .text
global asm_main
asm_main:
pusha
mov eax, [integer1] ; eax = int1
add eax, [integer2] ; eax = int1 + int2
mov [result], eax ; result = int1 + int2
call print_int ; print result
popa
mov eax, 0


ret ; end of program