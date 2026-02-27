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
    ; user inputs
    nameIn resb 100

    ; array vars
    num resd 1 ; user inp for how many elements to add
    array1 resb 100

    ; task 1 to go into two
    integer1 resd 1 ; first integer
    integer2 resd 1 ; second integer
    result resd 1 ; result

section .text
    global _start
    global asm_main


asm_main:
    
    ; jmp userInp2; only run array task3
; #################################################################################
    ; TASK 1: This code will read two integers, add them, and print the result
    ; to the console. It will also print the two integers to the console.

    ; This example code causes my code to break.
    ; Why.

    ; This uses variables: integer1, integer2, and result
    ; Registers: eax, ebx, ecx, edx

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
    
    ; attempt to fix the code

    ; The program was maybe adding numbers outside of the 8 bit range
    ; This then added this to the array addition task which caused a constant 


    ; This code clears the registers and memory locations, fixing array addition task
    mov eax, 0
    mov ebx, 0
    mov [result], eax
    mov [integer1], eax;eax
    mov [integer2], eax

    ; This worked, I believe it's because the add num is only supposed to take one num < 8 bits
    
    ; reset stack
; ################################################################################################
; Once you have spent a bit of time understanding how loops and conditionals can be implemented in
; assembler, write an assembler program that asks the user for their name and the number of times to
; print a welcome message. Test that the value is less than 100 and greater than 50 and then finally print
; out a welcome string that many times. Pretty an error message if the number if two large or small.
; 

; TASK 2 - Read name, read num to loop, validate it and then print welcome with name x times

    ; ask user for name
task2:
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

; ################################################################################################
; Write an assembler program that defines an array of 100 elements, initialize the array to the numbers
; 1 to 100, and then sum the that array, outputing the result.
; Finally, extend the previous program so that it asks the user to enter a range to sum, checking the
; range is valid, and then display the sum of that range.
;
; TASK 3 - Read num of elements, validate in range 50-100, generate filled aray and sum it


    userInp2:
            ; ask user for number of times to loop
            mov eax, askNumArray
            call print_string
            call read_int

            cmp eax, 50
            jl isError2
            cmp eax, 100
            jg isError2

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
    fillLoop:
        add [esi], eax
        inc eax
        add esi, 4
        loop fillLoop

    mov esi, array1
    mov ecx, 0
    mov eax, 0
    sumLoop:
        add eax, [esi]
        add esi, 4
        inc ecx
        cmp ecx, [num]
        jl sumLoop

    ; sub eax, 204 ; terrible workaround to fix the task1 interfering with task3
    call print_int
    call print_nl 

; END OF PROGRAM
; set exit code to 0
mov eax, 0
ret




