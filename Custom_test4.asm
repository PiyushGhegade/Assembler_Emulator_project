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


done:
    ldl 0
    ldl 1
    add
    stl 2
    ldl 2
    ldc result   ; Load address of result
    stnl 0       ; Store the result in the result address

HALT             ; End the program

; Data Section
a:  data 017      ; a = 17
b:  data 5       ; b = 5

quotient: data 0   ; result will store as quotien when a is divided by b
result: data 0   ; result will store remainder (a%b)


