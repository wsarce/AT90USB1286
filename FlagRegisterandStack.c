.INCLUDE “usb1286def.inc”             
.ORG $00
            
; (1) Set up the stack.                    
LDI R16, HIGH( RAMEND )                   
OUT SPH, R16                    
LDI R16, LOW( RAMEND )                    
OUT SPL, R16            
; (2) Observe the carry flag (C) after addition.                    
LDI R16, 128					
LDI R17, 128
ADD R16, R17 ; The result should be R16 = 256. CALL ClrSREG            
; (3) Observe the zero flag (Z).                    
LDI R16, 12					
LDI R17, 12
SUB R16, R17 ; The result should be R16 = 0. RCALL ClrSREG            
; (4) Observe the Negative Flag (N) and Sign Flag (S).                    
LDI R16, 0					
DEC R16 ; The result should be R16 = -1. RCALL ClrSREG          
; (5) Observe Two’s Complement Overflow Flag (V) and Negative Flag (N).                    
LDI R16, 127                   
INC R16                    
RCALL ClrSREG            
; (6) Observe the Half-Carry Flag (H).                    
LDI R16, $18      
ADD R16, R16       
RCALL ClrSREG					
Loop:					
; The result should be R16 = $30.					
; Loop forever here. This prevents program
; program execution from ‘falling through’ into ; the procedure section below. That would be
; catastrophic and cause your program to crash.					
RJMP Loop					
; ======================== Procedures ================================			
; ; ;					
Desc: The following procedure is used to clear all the flag bits
SREG so that the student can spot these changes when they occur. Note that it is not common practice to manually clear flag bits.					
ClrSREG:
PUSH R16 ; Save whatever’s in R16 into the ‘stack’.					
CLR R16 ; Clear R16 = 0.
OUT SREG, R16 ; Write this value to SREG register.					
POP R16 ; Restore R16’s value from the ‘stack’. RET ; Return from procedure.	
.EXIT 
