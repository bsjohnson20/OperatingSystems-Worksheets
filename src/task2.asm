%include "asm_io.inc"
extern printf
section .data
    format db "%s", 0

    ; Task 1 to put into task 2 because, that's what it asks!
    msg1 db "Enter a number: ", 0
    msg2 db "The sum of ", 0
    msg3 db " and ", 0
    msg4 db " is: ", 0

    ; mine vvvv

    ; questions
    askName db "Your name: ", 0
    askNum db "Times to loop: ", 0
    askNumArray db "Length of array to add (1-100)", 0

    ; outputs
    welcome db "Hello and welcome: ", 0

    ; errors
    numRangeErr db "Num not in range 50-100", 0

    ; array length
    arrayLen equ 100

section .bss
    nameIn resb 100
    num resd 1
    array1 resb 100

    ; task 1 to go into two
    integer1 resd 1 ; first integer
    integer2 resd 1 ; second integer
    result resd 1 ; result

section .text
    global _start
    global asm_main

asm_main:
    ; TASK 1 to put into two Slide 22
    pusha
    mov eax, msg1 ; note that this is a pointer!
    call print_string
    call read_int ; read the first integer
    mov [integer1], eax ; store it in memory
    mov eax, msg1 ; note that this is a pointer!
    call print_string
    call read_int ; read the second integer
    mov [integer2], eax ; store it in memory
    mov eax, [integer1] ; eax = first integer
    add eax, [integer2] ; eax += second integer
    mov [result], eax ; store the result
    mov eax, msg2 ; note that this is a pointer
    call print_string
    mov eax, [integer1] ; note that this is a value
    call print_int
    mov eax, msg3 ; note that this is a pointer
    call print_string
    mov eax, [integer2] ; note that this is a value
    call print_int
    mov eax, msg4 ; note that this is a pointer
    call print_string
    mov eax, [result] ; note that this is a value
    call print_int
    call print_nl
    popa
    mov eax, 0
    















    ; ask user for name
    mov eax, askName
    call print_string
    mov eax, nameIn
    call read_string
    
    userInp:
        ; ask user for number of times to loop
        mov eax, askNum
        call print_string
        call read_int

        cmp eax, 50
        jl isError
        cmp eax, 100
        jg isError

        jmp init
    
    isError:
        mov eax, numRangeErr
        call print_string
        mov eax, 0
        jmp userInp
    



    init:
        ; loop x times
        mov ecx, eax
        call print_int
        ; debug
        ; mov eax, nameIn
        ; call print_string

    printLoop:
        mov eax, welcome
        call print_string

        mov eax, 0

        mov eax, nameIn
        call print_string
        call print_nl
        loop printLoop








userInp2:
        ; ask user for number of times to loop
        mov eax, askNumArray
        call print_string
        call read_int

        cmp eax, 50
        jl isError
        cmp eax, 100
        jg isError

        mov [num], eax

        jmp valid
    
isError2:
    mov eax, numRangeErr
    call print_string
    mov eax, 0
    jmp userInp2


valid:

mov esi, array1      ; Set the pointer to the first element of the array
mov ecx, arrayLen   ; Set the loop counter
mov eax, 1
.fillLoop:
    add [esi], eax
    inc eax
    add esi, 4
    loop .fillLoop

mov esi, array1
mov ecx, 0
mov eax, 0
.sumLoop:
    add eax, [esi]
    add esi, 4
    inc ecx
    cmp ecx, [num]
    jl .sumLoop

call print_int
call print_nl 
ret




