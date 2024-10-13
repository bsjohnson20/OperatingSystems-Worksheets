%include "asm_io.inc"
    segment .data
    msg1 db "Enter a number: ", 0
    msg2 db "The sum of ", 0
    msg3 db " and ", 0
    msg4 db " is: ", 0
    msg5 db "Name please (4 letters): ", 0
    msg6 db "How many times looped: "
    msg7 db "Hello, ", 0
    rangeerror db "x is not within 50-100!", 0
    
    askint db "Enter an integer: ", 0
    finish db "Goodbye!", 0
    numRange db "Numbers to add (1-99): ", 0
segment .bss
    input resb 100
    integer1 resd 1 ; first integer
    integer2 resd 1 ; second integer
    result resd 1 ; result
    name resb 100 ; name
    offset resd 1 ; offset
    ; array for 100 integers
    array resd 100
    count resd 1 ; counter
segment .text
    global asm_main
asm_main:

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


; take a number to loop times, take a name from user and print that x times
; where x is the number taken from user.
mov eax, msg5
call print_string

call read_char
mov [name], eax
call read_char
mov [name+1], eax
call read_char
mov [name+2], eax
call read_char
mov [name+3], eax
call read_char
mov [name+4], eax
mov eax, msg5
call print_string
mov eax, name
call print_string
; mov eax, msg6
; call print_string
; call read_int
; mov ecx, eax

; test a number is in 50-100 range

jmp rangecorrect
rangecorrect:
    mov eax, askint
    call print_string ; ask for an integer
    call read_int
    mov ecx, eax ; store the result in ecx
    cmp eax, 50
    jl rangewrong
    cmp eax, 100
    jg rangewrong
    jmp repeat

rangewrong:
    mov eax, rangeerror
    call print_string
    call print_nl
    jmp rangecorrect

repeat: ; repeats the loop x times with printing "hello, {name}"
    mov eax, msg7
    call print_string
    mov eax, name
    call print_string
    call print_nl
    loop repeat



; fetch user input for how many numbers to add but must be less than 100 and more than 0


test: 
    ; rangeError
    mov eax, numRange
    call print_string
    call read_int
    cmp eax, 1
    jl rangeError
    cmp eax, 99
    jg rangeError
    mov [count], eax
    jmp finish2
    ; if in range then exit 

    ; mov ecx, eax

rangeError:
    mov eax, rangeerror
    call print_string
    call print_nl
    jmp test


; array length using user input from previously, then filling an array of len with 1-position in array. Finally summing the whole thing together and printing to user
finish2:
    
loopArray:
    mov ebx, [count] ; ebx is length of array
    mov eax, 1   ; eax is the number to fill array with
    mov esi, array ; array pointer
    mov ecx, ebx ; initialize ecx for fillloop

fillloop:
    inc eax
    mov [esi], eax
    add esi, 4
    
    loop fillloop
    
    mov ecx, ebx ; reinitialize ecx for sumloop
    mov eax, 0
    mov esi, array
sumloop:
    add eax, [esi]
    add esi, 4
    loop sumloop

    mov [offset], eax; store the result
    ; mov eax, msg8
    ; call print_string
    mov eax, [offset]
    call print_int
    call print_nl
    ; mov eax, exit
    ; call print_string ; test prints
    ; call print_nl
    mov eax, 0
    popa
    ret
    ; end of program


; honestly why couldn't we have just expanded on C++ doing the program. Assembly is agony