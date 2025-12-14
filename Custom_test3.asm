;/*****************************************************************************
;TITLE: Custom_Test1.asm	
;MULTIPLICATION_OF_TWO_NUMBER																															
;AUTHOR:   Piyush Ghegade
;Roll No:  2301AI52
;
;Declaration of Authorship
;This test file, is part of the miniproject of CS2102 at the 
;department of Computer Science and Engg, IIT Patna . 
;*****************************************************************************/
ldc 0x1000       ; Set the stack pointer start location at 0x1000
a2sp             ; Transfer this address to SP (stack pointer)

ldc a            ; Load address of a
ldnl 0
stl 0            ; Store base address of a in SP + 0

ldc b            ; Load address of b
ldnl 0
stl 1            ; Store base address of b in SP + 1

ldc 0            ; Initialize result = 0
stl 2            ; Store result in SP + 2

ldc 0            ; Initialize counter = 0 (this will count the number of additions)
stl 3            ; Store counter in SP + 3



mul_loop:
    ldl 0        ; A = 17 B = 0
    ldl 2        ; A = 0 B = 17

    sub          ; A = -17
    brlz done     ; move forward

    ldl 2        ; A = 0 B = -17
    ldl 1        ; A = 5 B = 0
    add          ; Add value of a to the result A = 5
    stl 2        ; Store updated result mem[SP + 2] = 5

    ldc 1        ; Load 1 A = 1 B = 5
    ldl 3        ; Load current counter A = 0 B = 1
    add          ; Increment counter A = 1 B = 1
    stl 3        ; Store updated counter mem[SP + 3] = 1

    br mul_loop  ; Repeat the loop

done:
    ldl 2
    ldl 1
    sub
    stl 2
    ldl 3
    ldc 1
    sub
    stl 3
    ldl 0
    ldl 2
    sub
    stl 2
    ldl 2
    ldc result   ; Load address of result
    stnl 0       ; Store the result in the result address
    ldl 3
    ldc quotient
    stnl 0

HALT             ; End the program

; Data Section
a:  data 17      ; a = 17
b:  data 5       ; b = 5

quotient: data 0   ; result will store as quotien when a is divided by b
result: data 0   ; result will store remainder (a%b)


